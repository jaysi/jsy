#define MAXCMD	20

#define INIT	0
#define STAT	1
#define ALLSTAT	2
#define SYNC	3
#define COMMENT	4
#define ID	5

//months
#define JAN	1001
#define FEB	1002
#define MAR	1003
#define APR	1004
#define MAY	1005
#define JUN	1006
#define JUL	1007
#define AUG	1008
#define SEP	1009
#define OCT	10010
#define NOV	10011
#define DEC	10012


static struct StrList {
	int no;
	const char *str;
} strlist[] = {
	{
	INIT, "init"}, {
	STAT, "stat"}, {
	ALLSTAT, "allstat"}, {
	SYNC, "sync"}, {
	COMMENT, "comment"}, {
	ID, "id"}, {
	JAN, "Jan"}, {
	FEB, "Feb"}, {
	MAR, "Mar"}, {
	APR, "Apr"}, {
	MAY, "May"}, {
	JUN, "Jun"}, {
	JUL, "Jul"}, {
	AUG, "Aug"}, {
	SEP, "Sep"}, {
	OCT, "Oct"}, {
	NOV, "Nov"}, {
	DEC, "Dec"}, {		
	0, 0}
};

int lookupstr(char *str);
