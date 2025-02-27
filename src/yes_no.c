
#include "vpulse.h"

int yes_no(char *pstring,int *preturn_value)
{
int i, found_yes_no_flag = 0;

char *plocal_string, *plocal_string_init;
char *pyes_string ="yes", *pno_string = "no";

if ((plocal_string = (char *) calloc(strlen(pstring) + 1,sizeof(char))) == NULL)
	{
	printf("Error allocating memory to plocal_string in yes_no.c!\n");
	exit(0);
	}

plocal_string_init = plocal_string;

strncpy(plocal_string,pstring,strlen(pstring));

while(*plocal_string != '\0')
	{
	*plocal_string = tolower(*plocal_string);
	plocal_string++;
	}

/* Restore pointers */

plocal_string = plocal_string_init;

if ((strcmp(plocal_string,pyes_string) == 0) || ((*plocal_string == 'y') && strlen(plocal_string) == 1))
	{
	found_yes_no_flag = 1;
	*preturn_value = 1;
	}
else
	{
	if ((strcmp(plocal_string,pno_string) == 0) || ((*plocal_string =='n') && strlen(plocal_string) == 1))
		{
		found_yes_no_flag = 1;
		*preturn_value = 0;
		}
	}
	
if (found_yes_no_flag == 1)
	return EXIT_SUCCESS;
else
	return EXIT_FAILURE;
}