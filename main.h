#ifndef _MAIN_H
#define _MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

//#define _SVID_SOURCE
#define _GNU_SOURCE
#include <dirent.h>

#include "if.h"

#ifndef MAXPATHNAME
#define MAXPATHNAME	4096
#endif

#ifdef _WIN32
#define DIR_DELIM		"\\"
#define DIR_DELIM_C		'\\'
#define JSYN_CREAT_FLAGS	"w+"
#define JSYN_OPEN_FLAGS		"r+"
#define JSYN_APPEND_FLAGS	"a+"
#define EOL			"\n\r"
#else
#define DIR_DELIM		"/"
#define DIR_DELIM_C		'/'
#define JSYN_CREAT_FLAGS	"w+"
#define JSYN_OPEN_FLAGS		"r+"
#define JSYN_APPEND_FLAGS	"a+"
#define EOL			"\n"
#endif

#define THIS_DIR		"."
#define PREV_DIR		".."
#define JSYN_DIR		".jsyn"
#define JSYN_ID_FILE		"ID"
#define JSYN_DATE_FILE		"DATE"
#define JSYN_FILE_LIST_FILE	"FILELIST"
#define JSYN_IGN_LIST_FILE	"IGNORE"
#define JSYN_COMMENT_FILE	"COMMENT"

extern int alphasort(const void *d1, const void* d2);

/*
 * MY TIME FORMAT: YYYY/MM/DD.HH:MM:SS
 * CTIME FORMAT: DAY MON DD HH:MM:SS YYYY
*/
extern char tbuf[30];

extern char wd[MAXPATHNAME]; //working directory
extern int level;

extern FILE* idf, *datef, *filelistf, *ignlistf, *commentf;

#endif
