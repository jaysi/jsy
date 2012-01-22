#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "if.h"

int lookupstr(char *str)
{
	struct StrList *c;

	for (c = strlist; c->str; c++)
		if (!strcmp(c->str, str))
			return c->no;
	return -1;
}

char* lookupstrid(int id){
	struct StrList *c;

	for (c = strlist; c->str; c++)
		if (c->no == id)
			return c->str;
	return NULL;	
}
