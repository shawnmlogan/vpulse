

#include "vpulse.h"

int find_modulation_type(char * pmodulation_type_string, int *modulation_type)
{

char *pam_string ="am", *ppm_string = "pm";
char *plocal_string, local_string[LINELENGTH + 1];
int i, found_modulation_type_flag = 0;

plocal_string = &local_string[0];


strncpy(plocal_string,pmodulation_type_string,LINELENGTH);

for(i = 0;i< strlen(plocal_string);i++)
	local_string[i] = tolower(local_string[i]);
	
if ((strcmp(plocal_string,pam_string) == 0) || (local_string[0] == 'a'))
	{
	*modulation_type = AM_MODULATION;
	found_modulation_type_flag = 1;
	}
else
	{
	if ((strcmp(plocal_string,ppm_string) == 0) || (local_string[0] == 'p'))
		{
		*modulation_type = PM_MODULATION;
		found_modulation_type_flag = 1;
		}
	else
		found_modulation_type_flag = 0;
	}
	
if (found_modulation_type_flag == 1)
	return EXIT_SUCCESS;
else
	return EXIT_FAILURE;
}