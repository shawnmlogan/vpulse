

#include "vpulse.h"

int find_noise_location(char * ppnoise_location_string, int *noise_location)
{

char *pfiltered_string ="filtered", *punfiltered = "unfiltered";
char *plocal_string, local_string[LINELENGTH + 1];
int i, found_noise_location_flag = 0;

plocal_string = &local_string[0];


strncpy(plocal_string,ppnoise_location_string,LINELENGTH);

for(i = 0;i< strlen(plocal_string);i++)
	local_string[i] = tolower(local_string[i]);
	
if ((strcmp(plocal_string,pfiltered_string) == 0) || (local_string[0] =='f'))
	{
	*noise_location = ENTERED_NOISE_AMP_IS_FILTERED;
	found_noise_location_flag = 1;
	}
else
	{
	if ((strcmp(plocal_string,punfiltered) == 0) || (local_string[0] =='u'))
		{
		*noise_location = ENTERED_NOISE_AMP_IS_UNFILTERED;
		found_noise_location_flag = 1;
		}
	}
	
if (found_noise_location_flag == 1)
	return EXIT_SUCCESS;
else
	return EXIT_FAILURE;
}