
#include "vpulse.h"

char *pnoise_type(int noise_type)
{
char temp[LINELENGTH + 1], *ptemp;
char *pnoise_type_0  = "gaussian", *pnoise_type_1 = "uniform"; 

ptemp = &temp[0];

switch(noise_type)
   {
   case 0:
      strcpy(ptemp,pnoise_type_0);
      break;
   case 1:
      strcpy(ptemp,pnoise_type_1);
      break;
    default:
       printf("Error! Did not recognize noise type in pnoise_type.c..exiting\n");
       exit(0);
    }
return(ptemp);
}
   
      
