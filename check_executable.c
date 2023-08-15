#include "vpulse.h"

int check_executable(char *pprogram_executable,char *preturn_string)
{
int found_executable_flag = 0;
char *pstring, string[LINELENGTH + 1];

FILE *fpw1;

pstring = &string[0];

sprintf(pstring,"command -v %s\n",pprogram_executable);

fpw1 = popen(pstring,"r");
fgets(preturn_string,LINELENGTH,fpw1);
remove_carriage_return(preturn_string);
pclose(fpw1);

if (strlen(preturn_string) < 2)
   {
   /* printf("Program executable \"%s\" is not found.\n",pprogram_executable); */
   found_executable_flag = 0;
   }
else
   {
   /* printf("strlen(preturn_string) is %lu.\n",strlen(preturn_string));
   printf("Program executable \"%s\" is located at \"%s\".\n",pprogram_executable,preturn_string); */
   found_executable_flag = 1;
   }

return found_executable_flag;

}
