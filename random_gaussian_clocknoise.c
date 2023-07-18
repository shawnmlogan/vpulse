
#include "vpulse.h"

double random_gaussian_clocknoise(double sigma, double average_value, unsigned int seed)
{
/*This routine computes gaussian random noise using a uniform noise distribution*/
/*The noise is computed and returned in unit intervals. The mean of the noise is 0.
The units of phase_val are in radians

# Reference: https://www.taygeta.com/random/gaussian.html

*/

static long i = 0;
double x1 = 0.0, x2 = 0.0, w = 0.0, y1 = 0.0, y2 = 0.0, noise = 0.0;
int iteration = 0;

if (i == 0)
	srand(seed);
	
do
   {
   iteration++;
   x1 = ((double) (RAND_MAX - rand()))/((double) RAND_MAX);
   x2 = ((double) (RAND_MAX - rand()))/((double) RAND_MAX);
   x1 = 2.0*x1 - 1.0;
   x2 = 2.0*x2 - 1.0;
   w = x1*x1 + x2*x2;
   }
while ( w >= 1.0 );

w = pow(-2.0*log(w)/w,0.50);
y1 = x1*w;
y2 = x2*w;

noise = average_value + y1*sigma;
i++;
return noise;
}
