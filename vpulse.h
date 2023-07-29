#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <sys/stat.h>

/*Add defines for debugging*/

/*Add #defines for version number*/

#define VERSION_NUMBER	1.1
#define VERSION_DATE "7/29/2023"

/*Add #defines for noise type*/

#define GAUSSIAN_NOISE 0
#define UNIFORM_NOISE 1

/* General #defines */

#define LINELENGTH 350
#define NUMBER_OF_PROGRESS_POINTS 10.0
#define TIME_ERROR_MAX 1E-15
#define SHOWPRPOINT 10
#define MAX_ITERATIONS 4

#define EPSILON 1.0E-12

#define NUMBER_OF_VALUE_STRINGS 12 /*Used for add_units arguments*/
#define LINELENGTH_OF_VALUE_STRING 20

#define OCTAVE 0
#define GNUPLOT 1

 
 struct coeff {
 double c1;
 double c2;
 double c3;
 double c4;
 double c5;
 double c6;
 double c7;
 double c8;
 double c9;
 double c10;
 double c11;
 };

 extern double pi;



/*Function prototypes*/


char * add_units(double value,int num_digits,char *suffix,char *pvalue_string);
void find_date(char *pdate_string,int max_characters);
int find_timestamp(char *pdate_string,int max_characters);

void remove_carriage_return(char *pline);
int check_executable(char *pprogram_executable,char *preturn_string);

/*Functions used to compute derivatives of diff equations*/

double f3(double tau, double vin, double vout);

/*Runge-Kutta related functions*/

int rkstep1(double tinit,double tfinal,double tau,double vin,double *pvcap,
double vmax, double vmin,double errmax,int iter);
void compute(struct coeff *pw,double R1,double R2,double R3,double C1,double C2, 
double RS1, double RS2,int ph);


/*Noise related functions*/

double random_gaussian_clocknoise(double sigma, double average_value, unsigned int seed);
double random_uniform_clocknoise(double range, double average_value, unsigned int seed);
char *pnoise_type(int noise_type);

/*Analysis data related functions*/

void sort_array_by_column_number(double *parray,int rows,int cols, int sort_column_number);

/*Define Cash-Karp parameters for embedded Runge-Kutta method - only for rkstep1()*/

#ifdef CASH_KARP
double a2 = 1.0/5.0;
double a3 = 3.0/10.0;
double a4 = 3.0/5.0;
double a5 = 1.0;
double a6 = 7.0/8.0;
double b21 = 1.0/5.0;
double b31 = 3.0/40.0;
double b32 = 9.0/40.0;
double b41 = 3.0/10.0;
double b42= 0.0 - (9.0/10.0);
double b43 = 6.0/5.0;
double b51 = 0.0 - (11.0/54.0);
double b52 = 5.0/2.0;
double b53 = 0.0 - (70.0/27.0);
double b54 = 35.0/27.0;
double b61 = 1631.0/55296.0;
double b62 = 175.0/512.0;
double b63 = 575.0/13824.0;
double b64 = 44275.0/110592.0;
double b65 = 253.0/4096.0;
double c1 = 37.0/378.0;
double c2 = 0.0;
double c3 = 250.0/621.0;
double c4 = 125.0/594.0;
double c5 = 0.0;
double c6 = 512.0/1771.0;
double s1 = 2825.0/27648.0;
double s2 = 0.0;
double s3 = 18575.0/48384.0;
double s4 = 13525.0/55296.0;
double s5 = 277.0/14336.0;
double s6 = 1.0/4.0;
#endif

#ifdef MAIN
double pi;
#endif

