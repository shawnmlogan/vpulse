

#include "vpulse.h"

int find_noise_type(char * pnoise_type_string, int *noise_type)
{

char *pgaussian_string ="gaussian", *puniform_string = "uniform", *psinusoidal_string = "sinusoidal";
char *plocal_string, local_string[LINELENGTH + 1];
int i, found_noise_type_flag = 0;

plocal_string = &local_string[0];


strncpy(plocal_string,pnoise_type_string,LINELENGTH);

for(i = 0;i< strlen(plocal_string);i++)
	local_string[i] = tolower(local_string[i]);
	
if ((strcmp(plocal_string,pgaussian_string) == 0) || (local_string[0] =='g'))
	{
	*noise_type = GAUSSIAN_NOISE;
	found_noise_type_flag = 1;
	}
else
	{
	if ((strcmp(plocal_string,puniform_string) == 0) || (local_string[0] =='u'))
		{
		*noise_type = UNIFORM_NOISE;
		found_noise_type_flag = 1;
		}
	else
		{
		if ((strcmp(plocal_string,psinusoidal_string) == 0) || (local_string[0] =='s'))
			{
			*noise_type = SINUSOIDAL_NOISE;
			found_noise_type_flag = 1;
			}
		else
			found_noise_type_flag = 0;
		}
	}
	
if (found_noise_type_flag == 1)
	return EXIT_SUCCESS;
else
	return EXIT_FAILURE;
}