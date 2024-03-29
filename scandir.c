/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)scandir.c	5.10 (Berkeley) 2/23/91";
#endif /* LIBC_SCCS and not lint */

/*
 * Scan the directory dirname calling select to make a list of selected
 * directory entries then sort using qsort and compare routine dcomp.
 * Returns the number of entries and a pointer to a list of pointers to
 * struct dirent (through namelist). Returns -1 if there were any errors.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

#define MAXNAMLEN FILENAME_MAX

#define ARRAY_CHUNK	10

/*
 * The DIRSIZ macro gives the minimum record length which will hold
 * the directory entry.  This requires the amount of space in struct dirent
 * without the d_name field, plus enough space for the name with a terminating
 * null byte (dp->d_namlen+1), rounded up to a 4 byte boundary.
 */
#undef DIRSIZ
#define DIRSIZ(dp) \
    ((sizeof (struct dirent) - (MAXNAMLEN+1)) + (((dp)->d_namlen+1 + 3) &~ 3))

#ifndef __P
#define __P(args) ()
#endif

int
scandir(dirname, namelist, select, dcomp)
	const char *dirname;
	struct dirent ***namelist;
	int (*select) __P((struct dirent *));
	int (*dcomp) __P((const void *, const void *));
{
	register struct dirent *d, *p, **names;
	register size_t nitems;
	struct stat stb;
	long arraysz;
	DIR *dirp;
	
	char this_dir[FILENAME_MAX];

	if ((dirp = opendir(dirname)) == NULL)
		return(-1);
	
	//if (fstat(dirp->dd_fd, &stb) < 0)
	//	return(-1);	

	//printf("stating %s ...", dirname);

	//if (stat(dirname, &stb) < 0)
	//	return(-1);
	
	////printf("done, stb.st_size = %u.\n", stb.st_size);

	/*
	 * estimate the array size by taking the size of the directory file
	 * and dividing it by a multiple of the minimum size entry. 
	 */
	
	//arraysz = (stb.st_size / 24);
	
	arraysz = ARRAY_CHUNK;
	names = (struct dirent **)malloc(arraysz * sizeof(struct dirent *));
	if (names == NULL)
		return(-1);

	nitems = 0;
	while ((d = readdir(dirp)) != NULL) {
		
		//printf("entered while, selecting.\n");
		
		if (select != NULL && !(*select)(d))
			continue;	/* just selected names */
		/*
		 * Make a minimum size copy of the data
		 */		
		
		p = (struct dirent *)malloc(DIRSIZ(d));
		
		//printf("getting p...");
				
		if (p == NULL)
			return(-1);
			
		//printf("done, d->d_name = %s\n", d->d_name);			
		
		p->d_ino = d->d_ino;
		p->d_reclen = d->d_reclen;
		p->d_namlen = d->d_namlen;
		//bcopy(d->d_name, p->d_name, p->d_namlen + 1);
		memcpy(p->d_name, d->d_name, p->d_namlen + 1);		
		/*
		 * Check to make sure the array has space left and
		 * realloc the maximum size.
		 */
		if (++nitems >= arraysz) {
			//if (fstat(dirp->dd_fd, &stb) < 0)
			//	return(-1);	/* just might have grown */
			
			//if (stat(dirname, &stb) < 0)
			//	return(-1);	/* just might have grown */
				
			//printf("here, nitems = %i >= arraysz = %i\n", nitems, arraysz);
			
			//arraysz += stb.st_size / 12;
			arraysz += ARRAY_CHUNK;
			names = (struct dirent **)realloc((char *)names,
				arraysz * sizeof(struct dirent *));			
				
			//printf("checking realloc\n");
			if (names == NULL)
				return(-1);
			//printf("OK\n");
		}
		names[nitems-1] = p;
		//printf("p->d_name = %s, names: %s\n", p->d_name, names[nitems - 1]->d_name);
	}
	closedir(dirp);
	if (nitems && dcomp != NULL)
		qsort(names, nitems, sizeof(struct dirent *), dcomp);
	*namelist = names;
	
	//printf("nitems = %i", nitems);
	
	return(nitems);
}

/*
 * Alphabetic order comparison routine for those who want it.
 */
int
alphasort(d1, d2)
	const void *d1;
	const void *d2;
{
	return(strcmp((*(struct dirent **)d1)->d_name,
	    (*(struct dirent **)d2)->d_name));
}

