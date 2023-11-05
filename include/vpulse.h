#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <sys/stat.h>

/*Add defines for debugging*/

/* #define DEBUG_NOISE
#define DEBUG_DUTY_CYCLE
#define COMPUTE_JITTER
#define DEBUG_SQUARE_WAVE
#define FILTER_VOUT */

#define VOUT_BANDWIDTH_MULTIPLIER 50.0
#define MAXIMUM_BANDWIDTH_MULTIPLIER 1000.0
#define COMPUTE_JITTER


/*Add #defines for version number*/

#define VERSION_NUMBER	2.01
#define VERSION_DATE "11/5/2023"

/* Add #defines for minimum number of points per period based on transition */
/* times and duty cycle */

#define MIN_NUM_POINTS_PER_TRANSIIION 10
#define MIN_NUM_POINTS_PER_ONTIME 10
#define MIN_NUM_POINTS_PER_PERIOD 100

/*Add #defines for noise type*/

#define GAUSSIAN_NOISE 0
#define UNIFORM_NOISE 1
#define SINUSOIDAL_NOISE 2

/*Add #defines for location of entered noise amplitude*/

#define ENTERED_NOISE_AMP_IS_FILTERED 0
#define ENTERED_NOISE_AMP_IS_UNFILTERED 1

/*Add #defines for modulation type*/

#define AM_MODULATION 0
#define PM_MODULATION 1

/* Add #defines and structure for interpolation */

/* #define DEBUG_INTERPOLATE_XY */
#define SKIP_IF_OVERRANGE 1
#define MINIMUM_INTERPOLATED_VALUE 0.0
#define MAXIMUM_INTERPOLATED_VALUE 1.0

typedef struct {
	double x,y;
	} xy_pair;

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
char * add_units_underscore(double value,int num_digits,char *suffix,char *pvalue_string);
void find_date(char *pdate_string,int max_characters);
int find_timestamp(char *pdate_string,int max_characters);

void remove_carriage_return(char *pline);
int check_executable(char *pprogram_executable,char *preturn_string);
int double_compare(const void* a, const void* b);
int parsestring_to_doubles_array(char *pinput_string,double *pdoubles_array,int *parray_size,int max_array_size);
double mean(double *x, long int N);
int check_inputs(double freq_Hz,double ttran_rise_percent,double ttran_fall_percent,double duty_cycle_percent, double vout_bandwidth_multiplier, double noise_amp, double noise_bandwidth_Hz, long int num_points_per_period, long int num_periods_to_plot, long int num_periods, char * pnoise_type_string, int *noise_type, char *pnoise_location_string, int *noise_location, char * pmodulation_type_string, int *modulation_type, double init_phase_degrees, double *init_phase_rad);
int append_filename_keep_N_characters(char *pfin, char *pfout_appended, char *pappended_string, int N, int max_num_characters);

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
int find_modulation_type(char * pmodulation_type_string, int *modulation_type);
int find_noise_type(char * pnoise_type_string, int *noise_type);
int find_noise_location(char * ppnoise_location_string, int *noise_location);

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

#define PLOTTING_ROUTINES_DIR "/Users/sml/cproj/vpulse/vpulse_v2p01_110523"
