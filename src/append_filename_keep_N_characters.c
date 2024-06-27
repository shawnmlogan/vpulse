# include "vpulse.h"

int append_filename_keep_N_characters(char *pfin, char *pfout_appended, char *pappended_string, int N, int max_num_characters)
{

int i = 0, error_flag = 0;
char *plocal_fin, *pextension_string, *plocal_fout_appended;

if (strlen(pfin) < N + 1)
	{
	printf("Value of N must be less than length of string pointed to by pfin in append_filename_keep_extension()\n");
	error_flag = 1;
	}
else
	{
	if ((plocal_fin = (char *) calloc(strlen(pfin) + 1,sizeof(char))) == NULL)
		{
		printf("Error allocating string for input filename in append_filename_keep_extension()\n");
		error_flag = 1;
		}
	else
		{
		if ((pextension_string = (char *) calloc(strlen(pfin) + 1,sizeof(char))) == NULL)
			{
			printf("Error allocating string for pextension_string in append_filename_keep_extension()\n");
			error_flag = 1;
			}
		else
			{
			if ((plocal_fout_appended = (char *) calloc(max_num_characters + 1,sizeof(char))) == NULL)
				{
				printf("Error allocating string for appended filename in append_filename_keep_extension()\n");
				error_flag = 1;
				}
			else
				{
				strcpy(plocal_fin,pfin);
			
				/* Create appended file name, remove N characters and append */
				/* "pappended_string" and replace N characters */
				
				for (i = 0; i < strlen(plocal_fin);i++)
					{
					if (i < strlen(plocal_fin) - N)
						{
						plocal_fout_appended[i] = plocal_fin[i];
						}
					else
						{
						plocal_fout_appended[i] = '\0';
						pextension_string[i - (strlen(plocal_fin) - N)] = plocal_fin[i];
						}
					}
				pextension_string[i - (strlen(plocal_fin) - N)] = '\0';
				strncat(plocal_fout_appended,pappended_string,max_num_characters);
				strncat(plocal_fout_appended,pextension_string,max_num_characters);
				strncpy(pfout_appended,plocal_fout_appended,max_num_characters);
				free(plocal_fin);
				free(plocal_fout_appended);
				free(pextension_string);
				}
			}
		}
	}

if (error_flag == 1)
	return EXIT_FAILURE;
else
	return EXIT_SUCCESS;
}
