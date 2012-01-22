#include "main.h"

extern FILE* datef, *commentf, *idf, *filelistf, *ignlistf;

int show_last_time(){
	
	time_t t;
	char str[30];

	while(!feof(datef)){
		if(fgets(str, 30, datef) < 0){
			perror("fgets()");
			return -1;
		}
	}
	
	printf("-> last update time: %s", str);
	
	return 0;
	
}

int show_all_time(){
	
	time_t t;
	char str[30];

	while(fgets(str, 30, datef)){
		printf("-> update time: %s", str);
	}	
	
	return 0;
	
}

int show_last_comment(){
	
	time_t t;
	char str[1024];

	while(!feof(commentf)){
		if(fgets(str, 1024, commentf) < 0){
			perror("fgets()");
			return -1;
		}		
	}
	
	printf("-> last comment: %s", str);
	
	return 0;
	
}

int show_all_comment(){
	
	time_t t;
	char str[1024];

	while(fgets(str, 1024, commentf)){
		printf("-> comment: %s", str);
	}	
	
	return 0;
	
}


int show_id(){
	
	time_t t;
	char str[1024];
	
	if(fgets(str, 1024, idf) < 0){
		perror("fgets()");
		return -1;
	}
	printf("-> $Id: %s\n", str);
	return 0;
	
}


int do_stat(char* target){
	
	char filepath[MAXPATHNAME];
	
	if(open_sys_files(filepath, JSYN_OPEN_FLAGS) < 0){
		perror("fopen()");
		return -1;		
	}
	
	printf("\t***\tSTAT\t***\n");
	
	if(show_id() < 0){
		return -1;
	}	
	
	if(show_last_time() < 0){
		return -1;
	}

	if(show_last_comment() < 0){
		return -1;
	}
	
	close_all();
	
	return 0;
	
}

int do_allstat(char* target){
	
	char filepath[MAXPATHNAME];
	
	if(open_sys_files(filepath, JSYN_OPEN_FLAGS) < 0){
		perror("fopen()");
		return -1;		
	}
	
	printf("\t***\tALL STAT\t***\n");
	
	if(show_id() < 0){
		return -1;
	}	
		
	if(show_all_time() < 0){
		return -1;
	}

	if(show_all_comment() < 0){
		return -1;
	}
	
	close_all();
	
	return 0;
	
}
