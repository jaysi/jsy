#include "main.h"

char tbuf[30];

char wd[MAXPATHNAME]; //working directory
int level;

FILE* idf, *datef, *filelistf, *ignlistf, *commentf;

void help(char* progname){
	printf("Usage:\n");
	printf("%s command <target-root> <source-roots>\n", progname);
	printf("commands are:\n");
	printf("\tinit -- initializes target\n");
	printf("\treinit -- initializes target, again!\n");
	printf("\tstat -- dumps info\n");
	printf("\tallstat -- dumps more info!\n");
	printf("\tsync -- synchronizes target with sources\n");
	printf("\tcomment -- puts comment on this date on target\n");
}

void push_dir(char* dir){
	//printf("pushing %s\n", dir);
	if(wd[0] == '\0'){
		strcpy(wd, dir);
		return;
	}
	if(wd[strlen(wd)] != DIR_DELIM_C) strcat(wd, DIR_DELIM);	
	strcat(wd, dir);
	//printf("result: %s\n", wd);
}

void pop_dir(){
	int i;
	//printf("poping from %s\n", wd);
	for(i = strlen(wd); i > -1; i--){
		if(wd[i] == DIR_DELIM_C){
			//printf("found %c at pos %i\n", DIR_DELIM_C, i);
			wd[i] = '\0';
			return;
		}
	}
}

void file_path(char* buf, char* filename){
	//printf("creating path from buf: %s, filename: %s\n", buf, filename);	
	strcpy(buf, wd);
	strcat(buf, DIR_DELIM);
	strcat(buf, filename);	
	//printf("result: %s\n", buf);
}

int enum_dir(char* root){
    struct dirent **namelist;
    int n;
    char filepath[MAXPATHNAME];
    struct stat sb;

	//time_t ti;
	struct tm *tp;
	//ti = time(NULL);
	//tp = localtime(&ti);
    
    //printf("working directory is %s, root is %s\n", wd, root);
    push_dir(root);
    
   n = scandir(wd, &namelist, 0, alphasort);
    if (n < 0){	
        perror("scandir()");
        printf("ERROR: working directory is < %s >\n", wd);
        return n;
    }else {
        while (n--) {
/*
#ifndef _WIN32		
	    if(	(namelist[n]->d_type == DT_DIR) &&
	    	(strcmp(namelist[n]->d_name, PREV_DIR)) &&
		(strcmp(namelist[n]->d_name, THIS_DIR)) &&
		(strcmp(namelist[n]->d_name, JSYN_DIR))){			
			
			level++;
			//push_dir(namelist[n]->d_name);
			//printf("[%s]\n", namelist[n]->d_name);
			enum_dir(namelist[n]->d_name);
		} else if(namelist[n]->d_type == DT_REG){			
			fprintf("%s%s%s\n", wd, DIR_DELIM, namelist[n]->d_name);
		}
#else
*/
	file_path(filepath, namelist[n]->d_name);
	//printf("stating %s\n", filepath);
	if(stat(filepath, &sb) != -1){
		switch (sb.st_mode & S_IFMT) {
			case S_IFDIR:
				
				if(!strcmp(namelist[n]->d_name, PREV_DIR) || 
				!strcmp(namelist[n]->d_name, THIS_DIR)||
				!(strcmp(namelist[n]->d_name, JSYN_DIR))) break;								
				
				level++;
				//push_dir(namelist[n]->d_name);
				
				printf("adding DIR: %s%s%s - modified %s",
					wd, DIR_DELIM, namelist[n]->d_name,					
					ctime(&sb.st_mtime)
					);					
				
				fprintf(filelistf,
					"D\t%s%s%s\t%u\t%s",
					wd, DIR_DELIM, namelist[n]->d_name,
					sb.st_size,
					ctime(&sb.st_mtime));
															
				enum_dir(namelist[n]->d_name);				
				break;
			case S_IFREG:
				
				tp = localtime(&sb.st_mtime);

				printf("adding FILE: %s%s%s - %u bytes - modified %s",
					wd, DIR_DELIM, namelist[n]->d_name,
					sb.st_size,
					ctime(&sb.st_mtime)
					);
				
				fprintf(filelistf,
					"F\t%s%s%s\t%u\t%s",
					wd, DIR_DELIM, namelist[n]->d_name,
					sb.st_size,
					ctime(&sb.st_mtime));
				break;
			default:
				break;
		}
	}
/*	
#endif
*/
            free(namelist[n]);
        }
        free(namelist);
    }
    
    pop_dir(root);    
}

int create_file_list(){
	return 0;
}

FILE* open_sys_file(char* file, char* flags){
	
	FILE* ret;
	char filepath[MAXPATHNAME];
	
	push_dir(JSYN_DIR);
	
	file_path(filepath, file);
	
	printf("openning %s\n");
	
	if(!(ret = fopen(filepath, flags))){
		perror("fopen()");
		printf("ERROR: could not open < %s >\n", filepath);
		return NULL;
	}	
	
	pop_dir();
	
	return ret;
}

int open_sys_files(char* filepath, char* flags){

	push_dir(JSYN_DIR);

	file_path(filepath, JSYN_ID_FILE);		
	if(!(idf = fopen(filepath, flags))){
		return -1;		
	}
	
	file_path(filepath, JSYN_DATE_FILE);
	if(!(datef = fopen(filepath, flags))){
		return -1;			
	}
	
	file_path(filepath, JSYN_FILE_LIST_FILE);
	if(!(filelistf = fopen(filepath, flags))){
		return -1;			
	}	
	
	file_path(filepath, JSYN_IGN_LIST_FILE);
	if(!(ignlistf = fopen(filepath, flags))){
		return -1;			
	}
	
	file_path(filepath, JSYN_COMMENT_FILE);
	if(!(commentf = fopen(filepath, flags))){
		return -1;			
	}	
	
	pop_dir();
	
	return 0;
	
}

int remake_sys_files(char* filepath){

	char* flags;
	
	struct stat st;

	push_dir(JSYN_DIR);
	
	file_path(filepath, JSYN_ID_FILE);
	if(stat(filepath, &st) < 0) flags = JSYN_CREAT_FLAGS;
	else flags = JSYN_OPEN_FLAGS;
	if(!(idf = fopen(filepath, flags))){
		return -1;		
	}
	
	file_path(filepath, JSYN_DATE_FILE);
	if(stat(filepath, &st) < 0) flags = JSYN_CREAT_FLAGS;
	else flags = JSYN_APPEND_FLAGS;
	if(!(datef = fopen(filepath, flags))){
		return -1;		
	}	

	file_path(filepath, JSYN_FILE_LIST_FILE);
	flags = JSYN_CREAT_FLAGS;	
	if(!(filelistf = fopen(filepath, flags))){
		return -1;		
	}	
	
	file_path(filepath, JSYN_IGN_LIST_FILE);
	if(stat(filepath, &st) < 0) flags = JSYN_CREAT_FLAGS;
	else flags = JSYN_OPEN_FLAGS;
	if(!(ignlistf = fopen(filepath, flags))){
		return -1;		
	}		

	file_path(filepath, JSYN_COMMENT_FILE);
	if(stat(filepath, &st) < 0) flags = JSYN_CREAT_FLAGS;
	else flags = JSYN_OPEN_FLAGS;
	if(!(commentf = fopen(filepath, flags))){
		return -1;		
	}		
	
	pop_dir();
	
	return 0;
	
}

void fill_date_file(){	
	
	time_t t;
	
	time(&t);
	
	fprintf(datef, "%s", ctime(&t));
	
	printf("INFO: last update set to %s", ctime(&t));
	
}

int fill_sys_files(char* root){	
	
	//nothing for id file
	
	//fill DATE
	fill_date_file();
	
	//fill file list
	enum_dir(root);
	
		
}

int check_sys_files(char* filepath){
	struct stat st;
	
	push_dir(JSYN_DIR);	
	
	file_path(filepath, JSYN_ID_FILE);
	if(stat(filepath, &st) < 0) return -1;
	
	file_path(filepath, JSYN_DATE_FILE);
	if(stat(filepath, &st) < 0) return -1;
	
	file_path(filepath, JSYN_FILE_LIST_FILE);
	if(stat(filepath, &st) < 0) return -1;
	
	file_path(filepath, JSYN_IGN_LIST_FILE);
	if(stat(filepath, &st) < 0) return -1;
	
	file_path(filepath, JSYN_COMMENT_FILE);
	if(stat(filepath, &st) < 0) return -1;	
	
	pop_dir();
	
	return 0;
			
}

void close_all(){
	if(idf)fclose(idf);
	if(datef) fclose(datef);
	if(filelistf) fclose(filelistf);
	if(ignlistf) fclose(ignlistf);
	if(commentf) fclose(commentf);
	idf = NULL;
	datef = NULL;
	filelistf = NULL;
	ignlistf = NULL;	
	commentf = NULL;
}

int init(char* target){
	
	struct stat st;
	char filepath[MAXPATHNAME];	
	
	//check to see if the path is already init'ed	
	
	file_path(filepath, JSYN_DIR);
	//printf(filepath);
	if(stat(filepath, &st) < 0){//not init'ed
		printf("INFO: initializing...\n");
		if(mkdir(filepath) < 0){
			printf("[FAIL]\n");
			printf("ERROR: could not create directory < %s >\n", filepath);
			perror("mkdir()");
			return -1;
		}		
		
		if(open_sys_files(filepath, JSYN_CREAT_FLAGS) < 0){
			printf("[FAIL]\n");
			printf("ERROR: could not create file < %s >\n", filepath);
			perror("fopen()");
			close_all();			
			return -1;
		}
		
		fill_sys_files(target);
		
		printf("[DONE]\n");
		
		close_all();
		
	} else {//
		printf("WARNING: already initialized, remaking system files...\n");
		
		if(remake_sys_files(filepath)< 0){
			printf("[FAIL]\n");
			printf("ERROR: could not remake file < %s >\n", filepath);
			perror("fopen()");
			return -1;				
		}		
		
		fill_sys_files(target);
		
		printf("[DONE]\n");
		
		close_all();		
		
	}	
	
	return 0;
}

int sync(char* target, char *argv[]){
	
	int i;
	
	//count sources from argv[3]	
	i = 3;
	while(argv[i]){
	
		i++;	
	}
	return 0;
}

int id(char* id){
	
	int ret;
	
	idf = open_sys_file(JSYN_ID_FILE, JSYN_APPEND_FLAGS);
	if(!idf) return -1;
	
	fprintf(idf, "%s", id);
	
	printf("INFO: $Id set to %s\n", id);
	
	fclose(idf);	
	
	return 0;
}

int main(int argc, char *argv[])
{
	
	struct stat st;
		
	if(argc < 3){
		help(argv[0]);
		return -1;
	}
	
	//init
	level = 0;
	//wd[0] = '\0';
	strcpy(wd, argv[2]);
	strcat(wd, DIR_DELIM);
	
	idf = NULL;
	datef = NULL;
	filelistf = NULL;
	ignlistf = NULL;
	
	if(stat(argv[2], &st) < 0){
		printf("ERROR: could not find target directory < %s >\n", argv[2]);
		return -1;
	}
	
	//parse command
	switch(lookupstr(argv[1])){
		
		case INIT:
			init(argv[2]);
			break;
		case STAT:
			do_stat(argv[2]);
			break;
		case ALLSTAT:
			do_allstat(argv[2]);
			break;			
		case SYNC:
			sync(argv[2], argv);
			break;
		case COMMENT:
			comment(argv[3]);
			break;
		case ID:
			id(argv[3]);
			break;
		default:
			printf("ERROR: could not understand command < %s >\n", argv[1]);
			break;
	}
		
	//enum_dir(argv[2]);
		
	return 0;
}
