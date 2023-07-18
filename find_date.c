#include "vpulse.h"

void find_date(char *pdate_string,int max_characters)
{

time_t timestamp;
struct tm *timeptr;

timestamp = time(NULL);
timeptr = localtime(&timestamp);
strftime(pdate_string,max_characters,"%m%d%y_%H_%M_%S", timeptr);

}
