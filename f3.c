
#include "vpulse.h"

double f3(double tau, double vin, double vout)

{
double f;

if (tau == 0)
   {
   printf("Attempted to divide by zero in f3.c - value of tau was zero...exiting..\n");
   exit(0);
   }
f = (1.0/(tau))*(vin - vout);

return(f);

}
