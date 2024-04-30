#include "vpulse.h"

void remove_carriage_return(char *pline)
{

char *ptempline, templine[LINELENGTH+1];
int i = 0;

ptempline = &templine[0];

if (strchr(pline,'\n') != NULL) /*Identify if string contains a newline character*/
   {
   strcpy(ptempline,pline);
   for (i=0;i < strlen(ptempline);i++)
      {
      if (templine[i] == '\n') /*Identify if string element is a newline character*/
         templine[i] = '\0';
      }
   /*printf("Removed newline character of input \"%s\" to form \"%s\".\n",pline,ptempline);*/
   strcpy(pline, ptempline);
   
   if (strchr(pline,'\r') != NULL) /*Identify if string contains a carriage return*/
	   {
	   strcpy(ptempline,pline);
	   for (i=0;i < strlen(ptempline);i++)
	      {
	      if (templine[i] == '\r') /*Identify if string element is a carriage return*/
	         templine[i] = '\0';
	      }
	   /*printf("Removed a carriage return from input \"%s\" to form \"%s\".\n",pline,ptempline);*/
	   strcpy(pline, ptempline);
	   }
   }

}
