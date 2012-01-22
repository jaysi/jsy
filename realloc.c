#ifdef _WIN32

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

#undef realloc

void *realloc(void *memblock, size_t size){
	
	size_t oldsize;
	void* ret;
	
	printf("entering my realloc\n");
	
	if(!memblock){
		if(!size) return NULL;
		return malloc(size);
	}
	
	if(!size){
		free(memblock);
		return NULL;
	}
	
	oldsize = _msize(memblock);
	
	if(oldsize == size){
		return memblock;
	} else {
		ret = malloc(size);		
		if(ret){
			memcpy(ret, memblock, oldsize);
			free(memblock);
		}
			
		return ret;
	}
	
	return NULL;
}

#endif
