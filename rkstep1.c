
#define CASH_KARP
#include "vpulse.h"

int rkstep1(double tinit,double tfinal,double tau,double vin,double *pvcap,\
double vmax, double vmin,double errmax,int iter)

{

/*This function computes the output signal due to a single
pole system with time constants tau using runge-kutta
embedded algorithm to provide fifth order runge-kutta integration
and step size adjustment.

vin = input voltage to network defined by RC time constant tau
pvcap = pointer to voltage on capacitor
tau = RC time constant of circuit = R(ohms) * C(farads)
tinit = time at start of iteration
tfinal = time at which new estimate of output voltage is required
errmax = maximum value of change to capacitor voltage (V)
iter = maximum of number of timesetp reductions allowed before error
vmax = maximum allowable voltage on capacitor before it is hard limited to vmax
vmin= minimum allowable voltage on capacitor before it is hard limited to vmin
message is given and routine exits*/

extern double b21,b31,b41,b51,b61,b32,b42,b52,b62,b43,b53,b63,b54,b64,b65;
extern double c1,c2,c3,c4,c5,c6,s1,s2,s3,s4,s5,s6;

double h,deltat,error,time,newstep,maxerror,dvcap;
double k1,k2,k3,k4,k5,k6;
double vcapnew,vcapnewb,vcap;
int i, status = 0, reducestep;

/*Copy capacitor value to local variable*/

vcap = *pvcap;

/*Compute initial derivative*/

dvcap = f3(tau,vin,vcap);

/*Define initial step size (maximum) and stop time*/

time = tinit;
deltat = tfinal - tinit;

/*Start integration with h = deltat*/

h = deltat;

/*initialize loop counter i*/

i = 1;

/*Perform integration until timestep causes time to exceed tfinal*/

while (time != tfinal )
   {
   k1 = h*f3(tau,vin,vcap);
   k2 = h*f3(tau,vin,vcap + b21*k1);
   k3 = h*f3(tau,vin,vcap + b31*k1 + b32*k2);  
   k4 = h*f3(tau,vin,vcap + b41*k1 + b42*k2 + b43*k3);
   k5 = h*f3(tau,vin,vcap + b51*k1 + b52*k2 + b53*k3 + b54*k4);
   k6 = h*f3(tau,vin,vcap + b61*k1 + b62*k2 + b63*k3 + b64*k4 + b65*k5);

/*Compute next value and error*/

   vcapnew = vcap + c1*k1 + c2*k2 + c3*k3 + c4*k4 + c5*k5 + c6*k6;
   vcapnewb = vcap + s1*k1 + s2*k2 + s3*k3 + s4*k4 + s5*k5 + s6*k6;
   
   /*Verify that errors are less than permissible errors*/
   
   reducestep = 0;
   newstep = h;
    
   if (fabs(vcapnew - vcapnewb) > fabs(errmax + fabs(h*dvcap)))
      {
      reducestep = 1;
      error = fabs(vcapnew - vcapnewb);
      maxerror = fabs(errmax + fabs(h*dvcap));
      newstep = h * pow(maxerror/error, 0.25);
      }

   /*If must reduce timestep, verify that iter has not been exceeded*/
   
   if (reducestep == 1)
      {
      /*Verify that number of times timestep has been reduced is less than
      iter - if it has terminate execution with return status 1 and print
      error message*/
	
      if (i >= iter ) 
         {
         printf("Iteration limit exceeded at time %2.4e, execution stopped!\n",
         time);
         printf("Reduced timestep %d times and maximum error was still too large.\n",
         iter);
         status=1;
         return(status);
         }	
      /*Reduce timestep to appropriate value depending on measured error,
      do not allow stepsize to be reduced more than a factor of 10*/
      
	  if (newstep < h/10.0)
	     h = h/10.0;
	  else
	     h = newstep;
         i++;
      }	
      else
         {
      /*If no error, reset i=1, advance time by increment h*/

         if ((time + h > tfinal) && (time != tfinal))
            {
            h = tfinal - time;
            time = tfinal;
            }
         else
           {
            time = time + h;
            }
         i = 1;
         
      /*Limit signal value at capacitor to vcmax and vcmin*/

         if (vcapnew > vmax)
            vcapnew = vmax;
         if (vcapnew < vmin)
            vcapnew = vmin;
            
      /*Assign new value to state variables*/

         *pvcap = vcapnew;
         }	

      }

return(status);

}




