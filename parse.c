#include "main.h"

/*
1	January	31
2	February	28 or 29
3	March	31
4	April	30
5	May	31
6	June	30
7	July	31
8	August	31
9	September	30
10	October	31
11	November	30
12	December	31
*/

/*
 * MY TIME FORMAT: YYYY/MM/DD.HH:MM:SS
 * CTIME FORMAT: DAY MON DD HH:MM:SS YYYY
*/

#define DATESEP	"/"
#define DATESEP_C	'/'

int mon_to_mm(char* mon){
	return lookupstr(mon);	
}

char* mm_to_mon(int mm){
	return lookupstrid(mm);
}

char* ctime_to_mytime(char* ct){	
		
	return tbuf;	
}

char* mytime_to_ctime(char* my){
	
	return tbuf;
}

int parse_ctime(

int cmp_ctime(char* ct1, char* ct2){
	
}
