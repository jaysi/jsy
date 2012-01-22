#include "main.h"

int comment(char* comment){
	
	time_t t;
	
	int ret;
	
	commentf = open_sys_file(JSYN_COMMENT_FILE, JSYN_APPEND_FLAGS);
	if(!commentf) return -1;
	
	time(&t);
	
	fprintf(commentf, "%s @ %s", comment, ctime(&t));
	
	printf("INFO: last comment set to %s @ %s", comment, ctime(&t));
	
	fclose(commentf);
	
	return 0;
}
