
#include "vpulse.h"

double random_uniform_clocknoise(double range, double average_value, unsigned int seed)
{
/*This routine computes a random noise component using a uniform noise distribution*/
/*The noise is computed and returned in unit intervals. The range of noise is from
-range/2 to to +range/2. The units of phase_val are in radians*/

static long i = 0;
double noise = 0.0;

if (i == 0)
	srand(seed);
	
noise = average_value + range * ((double) (rand() - RAND_MAX/2.0))/(0.0 + RAND_MAX);
i++;
return noise;
}
