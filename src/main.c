#define MAIN

#include "vpulse.h"

int main(int argc, char **argv)
{

/*Define variables*/

double *time_sec, *vsin, *vsin_pm, *vsq, *vout, *vout_filtered, *pm_noise, *am_noise, *tmp_double_ptr;
double *vth_cross_rise, *vth_cross_fall, *duty_cycle, mean_du = 0.0;
double init_phase_rad = 0.0, init_phase_degrees = 0.0;
double prpoint, prtime, TSTART = 0.0;
double tau=EPSILON, tau_noise_bandwidth_sec;
double noise = 0.0, noise_amp = 0.10, unfiltered_noise_amp = 0.0, maximum_delta_noise = 0.0, noise_bandwidth_Hz;
double noise_vthreshold, filtered_noise = 0.0, last_filtered_noise = 0.0, noise_max = -999.0, noise_min = 999.0;
double freq_Hz, per_sec, ttran_rise_percent, ttran_rise, ttran_fall_percent, ttran_fall;
double vthreshold, duty_cycle_percent, delta_time;
double vout_threshold = .50,mean_vout_filtered_du = 0.0, vout_filtered_max = -999.0, vout_filtered_min = 999.0;
double vout_bandwidth_multiplier = VOUT_BANDWIDTH_MULTIPLIER, tau_vout_sec, vout_filtered_cap = 0.0;

/* Coefficients to set sinusoid threshold for an input duty cycle */

double coeffi[] = {9.726743609632e-01,4.212749865460e-03,-7.100201979438e-04,4.733679173627e-06};

/* Coefficients to set range of input noise for given input bandwidth to set */
/* desired filtered output noise (pp) */

double c0 = 2.555552820758e+01, m = 5.757636118011e-01;
double errmax = 100.0;

int noise_type = GAUSSIAN_NOISE;
int noise_location = ENTERED_NOISE_AMP_IS_FILTERED;
int modulation_type = AM_MODULATION;
int end_flag = 0;
int num_samples_moving_average = 12;

unsigned int seed;

long int i, j, k, l;
long int num_points_per_period, num_periods, settling_periods, num_periods_to_plot, num_crossings;
long int length_vth_cross_rise = 0, length_vth_cross_fall = 0;
long int length_vout_vth_cross_rise = 0, length_vout_vth_cross_fall = 0; 
long int noise_counter = 0, time_point_counter;

char base_filename[LINELENGTH + 1], *pbase_filename;
char fnameout[LINELENGTH + 1], *pfnameout;
char fnameout_jitter[LINELENGTH + 1], *pfnameout_jitter;
char line1[LINELENGTH+1], *pline1, *pinput_string, input_string[LINELENGTH + 1], *ptemp_string, temp_string[LINELENGTH + 1];
char *pfreq_value, freq_value[LINELENGTH + 1], *pnoise_bandwidth_value, noise_bandwidth_value[LINELENGTH + 1];

char *pnoise_type_string, noise_type_string[LINELENGTH+1];
char *pnoise_location_string, noise_location_string[LINELENGTH+1];
char *pmodulation_type_string, modulation_type_string[LINELENGTH+1];
xy_pair *pxy, *px1y1;
char *plinear="linear";
char *ptimestamp,timestamp[LINELENGTH + 1];
char *pmodulation_string, modulation_string[LINELENGTH + 1];
char *pnoise_prefix, noise_prefix[2];

char fileout_octave_command_line[LINELENGTH+1], *pfileout_octave_command_line;

char octave_command_1[LINELENGTH + 1], octave_command_2[LINELENGTH + 1], octave_command_3[LINELENGTH + 1];
char *poctave_command_1, *poctave_command_2, *poctave_command_3;

char value_string[NUMBER_OF_VALUE_STRINGS][LINELENGTH_OF_VALUE_STRING + 1];
char *ptitle_string, title_string[LINELENGTH + 1];
char *poctave = "octave",*poctave_path,octave_path[LINELENGTH + 1];
char *pgnuplot = "gnuplot",*pgnuplot_path,gnuplot_path[LINELENGTH + 1];
char *pgimp = "gimp",*pgimp_path,gimp_path[LINELENGTH + 1];
char *pjitterhistv16 = "jitterhistv16",*pjitterhistv16_path,jitterhistv16_path[LINELENGTH + 1];
char *ppsd_sppowr = "psd_sppowr",*ppsd_sppowr_path,psd_sppowr_path[LINELENGTH + 1];
char plot_preference[LINELENGTH + 1];


FILE *fpw1,*fp_noise;

/*Define pi */

pi = 2.0 * asin(1.0);

/*Initialize pointers*/

pbase_filename = &base_filename[0];
pfnameout = &fnameout[0];
pfnameout_jitter = &fnameout_jitter[0];
pfreq_value = &freq_value[0];
pnoise_bandwidth_value = &noise_bandwidth_value[0];

pfileout_octave_command_line = &fileout_octave_command_line[0];
pline1 = &line1[0];
ptimestamp = &timestamp[0];
pinput_string = &input_string[0];
ptemp_string = &temp_string[0];
pmodulation_type_string = &modulation_type_string[0];
pmodulation_string = &modulation_string[0];
pnoise_type_string = &noise_type_string[0];
pnoise_location_string = &noise_location_string[0];
pnoise_prefix = &noise_prefix[0];

poctave_command_1 = &octave_command_1[0];
poctave_command_2 = &octave_command_2[0];
poctave_command_3 = &octave_command_3[0];
ptitle_string = &title_string[0];
poctave_path = &octave_path[0];
pgnuplot_path = &gnuplot_path[0];
pgimp_path = &gimp_path[0];
pjitterhistv16_path = &jitterhistv16_path[0];
ppsd_sppowr_path = &psd_sppowr_path[0];

printf("\nvpulse v%.2f %s\n\n",VERSION_NUMBER,VERSION_DATE);

if ((argc != 15) && (argc != 10))
	{
	for (i = 0;i<argc;i++)
		{
		printf("Argument %ld: %s\n",i,argv[i]);
		}
	printf("Detected %d arguments for command line use of \"vpulse\"!\n\n",argc);
	printf("If the waveform is not amplitude or phase modulated, 9 arguments are required.\n\n");
	printf("Usage with command line:\nvpulse <input_square_wave_freq_hz> <initial_phase (degrees)>\n");
	printf("<trise_per_cent_period> <tfall_per_cent_period> <duty_cycle_percent>\n");
	printf("<vout RC filter bandwidth multiplier (0 for no bandlimitng)>\n");
	printf("<num_points_per_period> <num_periods> <num_periods_to_plot>\n\n");

	printf("If the waveform includes amplitude or phase modulation, 13 arguments are required.\n");
	printf("Following the command line syntax for this second form of \"vpulse\",\nyou will be prompted for all inputs.\n\n");
		
	printf("Usage with command line:\nvpulse <input_square_wave_freq_hz> <initial_phase (degrees)>\n");
	printf("<trise_per_cent_period> <tfall_per_cent_period> <duty_cycle_percent>\n");
	printf("<vout RC filter bandwidth multiplier (0 for no bandlimitng)>\n");
	printf("<num_points_per_period> <num_periods> <num_periods_to_plot>\n");
	printf("<noise_amplitude (modulation index between 0.0 and 1.0)>\n");
	printf("<noise_amplitude location (filtered (f) or unfiltered(u)))>\n");
	printf("<noise_type (sinusoidal, gaussian, or uniform)> <noise_bandwidth_Hz>\n");
	printf("<modulation_type (AM or PM)>\n\n");
			
	printf("Enter square wave freq (Hz):\n");
	fgets(pinput_string,LINELENGTH,stdin);
	remove_carriage_return(pinput_string);
	freq_Hz = atof(pinput_string);
	
	printf("Enter initial phase (degrees):\n");
	fgets(pinput_string,LINELENGTH,stdin);
	remove_carriage_return(pinput_string);
	init_phase_degrees = atof(pinput_string);
	
	printf("Enter rise time (percent of period):\n");
	fgets(pinput_string,LINELENGTH,stdin);
	remove_carriage_return(pinput_string);
	ttran_rise_percent = atof(pinput_string);

	printf("Enter fall time (percent of period):\n");
	fgets(pinput_string,LINELENGTH,stdin);
	remove_carriage_return(pinput_string);
	ttran_fall_percent = atof(pinput_string);

	printf("Enter duty cycle (percent of period):\n");
	fgets(pinput_string,LINELENGTH,stdin);
	remove_carriage_return(pinput_string);
	duty_cycle_percent = atof(pinput_string);

	printf("Enter vout RC filter frequency multiplier (enter 0 for no bandlimiting):\n");
	fgets(pinput_string,LINELENGTH,stdin);
	remove_carriage_return(pinput_string);
	vout_bandwidth_multiplier = atof(pinput_string);
	
	printf("Enter number of points per period:\n");
	fgets(pinput_string,LINELENGTH,stdin);
	remove_carriage_return(pinput_string);
	num_points_per_period = atol(pinput_string);

	printf("Enter number of periods:\n");
	fgets(pinput_string,LINELENGTH,stdin);
	remove_carriage_return(pinput_string);
	num_periods = atol(pinput_string);

	printf("Enter the number of periods to plot:\n");
	fgets(pinput_string,LINELENGTH,stdin);
	remove_carriage_return(pinput_string);
	num_periods_to_plot = atol(pinput_string);

	printf("Enter noise amplitude (modulation index between 0.0 and less than 1.0):\n");
	fgets(pinput_string,LINELENGTH,stdin);
	remove_carriage_return(pinput_string);
	noise_amp = atof(pinput_string);

	printf("Enter noise amplitude location (filtered (f) or unfiltered (u)):\n");
	fgets(pnoise_location_string,LINELENGTH,stdin);
	remove_carriage_return(pnoise_location_string);
	
	printf("Enter noise type (sinusoidal (s), gaussian (g), uniform) (u):\n");
	fgets(pnoise_type_string,LINELENGTH,stdin);
	remove_carriage_return(pnoise_type_string);

	printf("Enter noise bandwidth or frequency of sinusoidal noise (Hz):\n");
	fgets(pinput_string,LINELENGTH,stdin);
	remove_carriage_return(pinput_string);
	noise_bandwidth_Hz = atof(pinput_string);
	
	printf("Enter modulation type (AM (a) or PM (p)):\n");
	fgets(pmodulation_type_string,LINELENGTH,stdin);
	remove_carriage_return(pmodulation_type_string);

	}
else
	{
	if (argc == 15)
		{
		strncpy(pinput_string,argv[1],LINELENGTH);
		freq_Hz = atof(pinput_string);
		
		strncpy(pinput_string,argv[2],LINELENGTH);
		init_phase_degrees = atof(pinput_string);
		
		strncpy(pinput_string,argv[3],LINELENGTH);
		ttran_rise_percent = atof(pinput_string);
		
		strncpy(pinput_string,argv[4],LINELENGTH);
		ttran_fall_percent = atof(pinput_string);
	
		strncpy(pinput_string,argv[5],LINELENGTH);
		duty_cycle_percent = atof(pinput_string);

		strncpy(pinput_string,argv[6],LINELENGTH);
		vout_bandwidth_multiplier = atof(pinput_string);
			
		strncpy(pinput_string,argv[7],LINELENGTH);
		num_points_per_period = atol(pinput_string);
	
		strncpy(pinput_string,argv[8],LINELENGTH);
		num_periods  = atol(pinput_string);
	
		strncpy(pinput_string,argv[9],LINELENGTH);
		num_periods_to_plot = atol(pinput_string);
		
		strncpy(pinput_string,argv[10],LINELENGTH);
		noise_amp = atof(pinput_string);
		
		strncpy(pnoise_location_string,argv[11],LINELENGTH);
		remove_carriage_return(pnoise_location_string);
	
		strncpy(pinput_string,argv[12],LINELENGTH);
		strcpy(pnoise_type_string,pinput_string);
	
		strncpy(pinput_string,argv[13],LINELENGTH);
		noise_bandwidth_Hz = atof(pinput_string);
	
		strncpy(pinput_string,argv[14],LINELENGTH);
		strcpy(pmodulation_type_string,pinput_string);
		}
	else
	   {
		strncpy(pinput_string,argv[1],LINELENGTH);
		freq_Hz = atof(pinput_string);
		
		strncpy(pinput_string,argv[2],LINELENGTH);
		init_phase_degrees = atof(pinput_string);
		
		strncpy(pinput_string,argv[3],LINELENGTH);
		ttran_rise_percent = atof(pinput_string);
		
		strncpy(pinput_string,argv[4],LINELENGTH);
		ttran_fall_percent = atof(pinput_string);
	
		strncpy(pinput_string,argv[5],LINELENGTH);
		duty_cycle_percent = atof(pinput_string);

		strncpy(pinput_string,argv[6],LINELENGTH);
		vout_bandwidth_multiplier = atof(pinput_string);
			
		strncpy(pinput_string,argv[7],LINELENGTH);
		num_points_per_period = atol(pinput_string);
	
		strncpy(pinput_string,argv[8],LINELENGTH);
		num_periods  = atol(pinput_string);
	
		strncpy(pinput_string,argv[9],LINELENGTH);
		num_periods_to_plot = atol(pinput_string);
		
		noise_amp = 0.0;
   	}
	}

if (check_vpulse_inputs(freq_Hz,ttran_rise_percent,ttran_fall_percent,duty_cycle_percent,vout_bandwidth_multiplier,noise_amp,noise_bandwidth_Hz,num_points_per_period,num_periods_to_plot,num_periods,pnoise_type_string,&noise_type,pnoise_location_string,&noise_location,
pmodulation_type_string,&modulation_type,init_phase_degrees,&init_phase_rad) != EXIT_SUCCESS)
		{
		printf("Correct inputs and try again...\n");
		exit(0);
		}

/* Set seed for random number generators */

seed = time(NULL);

if (vout_bandwidth_multiplier > 0.0)
	tau_vout_sec = 1.0/(2.0*pi*vout_bandwidth_multiplier*freq_Hz);
else
	tau_vout_sec = 0.0;

 /* Extend analysis time to allow for settling time of first-order post filter by settling_periods periods */
if (tau_vout_sec != 0)
	{
	settling_periods = (long int) ceil(5.0/vout_bandwidth_multiplier);
	}
else
	settling_periods = 0;

/* Compute sinusoidal threshold based on 3rd order polynomial */
	
	vthreshold = 0.0;
	for(i = 0; i< 4; i++)
		vthreshold = vthreshold + coeffi[i]*pow(duty_cycle_percent,i);
#ifdef DEBUG_DUTY_CYCLE
	printf("Computed vthreshold = %.4f from duty cycle of %.2f%%.\n",vthreshold,duty_cycle_percent);
#endif

printf("Analysis parameters:\n");
printf("Square wave input frequency = %s\n",add_units(freq_Hz,3,"Hz",value_string[0]));
printf("Initial phase = %.1f degrees\n",init_phase_degrees );
printf("Rise time = %.1f%% of period\n",ttran_rise_percent);
printf("Fall time = %.1f%% of period\n",ttran_fall_percent);
printf("Duty cycle = %.1f%% of period\n",duty_cycle_percent);
printf("Square wave RC filter bandwidth multiplier = %.2f\n",vout_bandwidth_multiplier);
printf("num_points_per_period = %ld\n",num_points_per_period);
printf("num_periods = %ld\n",num_periods);
printf("num_periods_to_plot = %ld\n",num_periods_to_plot);
printf("modulation_index (noise_amp) = %s\n",add_units(noise_amp,1,"",value_string[0]));

if (noise_amp != 0.0)
	{
	if (noise_type != SINUSOIDAL_NOISE)
		{
		if (noise_location == ENTERED_NOISE_AMP_IS_FILTERED)
			printf("noise amplitude location = post filter\n");
		else
			{
			if (noise_location == ENTERED_NOISE_AMP_IS_UNFILTERED)
				printf("noise amplitude location = pre-filter\n");
			else
				printf("noise amplitude location = unknown\n");
			}
		}
	
	if (noise_type == GAUSSIAN_NOISE)
		{
		printf("noise type = gaussian\n");
		sprintf(pnoise_prefix,"g");
		}
	else
		{
		if (noise_type == UNIFORM_NOISE)
			{
			printf("noise type = uniform\n");
			sprintf(pnoise_prefix,"u");
			}
		else
			{
			if (noise_type == SINUSOIDAL_NOISE)
				{
				printf("noise type = sinusoidal\n");
				sprintf(pnoise_prefix,"s");
				}
			else
				printf("noise type = unknown\n");
			}
		}
	
	if (noise_type != SINUSOIDAL_NOISE)
		printf("noise_bandwidth = %s\n",add_units(noise_bandwidth_Hz,2,"Hz",value_string[0]));
	else
		printf("Sinusoidal modulation frequency = %s\n",add_units(noise_bandwidth_Hz,2,"Hz",value_string[0]));
		
	if (modulation_type == AM_MODULATION)
		{
		printf("modulation type = AM\n");
		sprintf(pmodulation_string,"Amplitude");
		}
	else
		{
		if (modulation_type == PM_MODULATION)
			{
			printf("modulation type = PM\n");
			sprintf(pmodulation_string,"Phase");
		}
		
		else
			printf("modulation type = unknown\n");
		}
}
	/* Find timestamp to append to filenames */

	find_timestamp(ptimestamp,LINELENGTH);

/* Create strings for noise_bandwidth and freq_Hz for filenames */

sprintf(pfreq_value,"%s",add_units_underscore(freq_Hz,0,"Hz",value_string[0]));
sprintf(pnoise_bandwidth_value,"%s",add_units_underscore(noise_bandwidth_Hz,0,"Hz",value_string[0]));

if (noise_amp != 0.0)
	{
	
	switch (modulation_type)
		{
		case AM_MODULATION:
			sprintf(pbase_filename,"square_wave_%s_ttran_rise_%.0f_fall_%.0f_percent_du_%.0f_am_%snoise_amp_%.0fm_noise_bw_%s_%s",
			pfreq_value,ttran_rise_percent,ttran_fall_percent,duty_cycle_percent,pnoise_prefix,noise_amp/1e-03,pnoise_bandwidth_value,ptimestamp);
      		break;
      	case PM_MODULATION:
			sprintf(pbase_filename,"square_wave_%s_ttran_rise_%.0f_fall_%.0f_percent_du_%.0f_pm_%snoise_amp_%.0fm_noise_bw_%s_%s",
			pfreq_value,ttran_rise_percent,ttran_fall_percent,duty_cycle_percent,pnoise_prefix,noise_amp/1e-03,pnoise_bandwidth_value,ptimestamp);
      		break;
      	default:
      		printf("In main.c, did NOT recognize modulation type! Exiting...\n");
            exit(0);
         }
	}
else
	sprintf(pbase_filename,"square_wave_%s_ttran_rise_%.0f_fall_%.0f_percent_du_%.0f_%s",
	pfreq_value,ttran_rise_percent,ttran_fall_percent,duty_cycle_percent,ptimestamp);

	sprintf(pfnameout,"%s.csv",pbase_filename);
	printf("Output filename is \"%s\".\n",pfnameout);
            
   /*Start loop*/
   
	per_sec = 1.0/freq_Hz;		
   ttran_rise = (ttran_rise_percent/100.0)*per_sec;
	ttran_fall = (ttran_fall_percent/100.0)*per_sec;
	if (noise_amp != 0.0)  
		tau_noise_bandwidth_sec = 1.0/(2*pi*noise_bandwidth_Hz);
	else
		tau_noise_bandwidth_sec = 1.0/(2*pi*noise_bandwidth_Hz);	
	delta_time = per_sec/((double) num_points_per_period - 0.0);	
	prpoint = delta_time;
	
	TSTART = delta_time*( (double) (num_points_per_period*(num_periods - num_periods_to_plot)));
	printf("Analysis is for %s, data printed starting at %s.\n",add_units(delta_time*( (double) num_points_per_period*num_periods),3,"s",value_string[0]),add_units(TSTART,3,"s",value_string[1]));

	if (noise_amp != 0.0)
		{
		/* Compute input (unfiltered) noise for given bandwidth and desired output noise for gaussian and uniform noise */
		
		if (noise_type != SINUSOIDAL_NOISE)
			{
			if (noise_location == ENTERED_NOISE_AMP_IS_FILTERED)
				unfiltered_noise_amp = 0.50*10.0*noise_amp/(c0*pow(noise_bandwidth_Hz*delta_time,m));
			else
				{
				if (noise_location == ENTERED_NOISE_AMP_IS_UNFILTERED)
					unfiltered_noise_amp = noise_amp;
				else
					{
					printf("Unknown noise injection location type, setting injection location to after filter.\n");
					unfiltered_noise_amp = 0.50*10.0*noise_amp/(c0*pow(noise_bandwidth_Hz*delta_time,m));
					}
				}
			}		
		#ifdef DEBUG_NOISE
			if (noise_location == ENTERED_NOISE_AMP_IS_FILTERED)
				{
				printf("Setting unfiltered_noise_amp to %.4f from noise_amp of %.4f\nfor noise bandwidth of %s and sampling frequency of %s.\n",
				unfiltered_noise_amp,noise_amp, add_units(noise_bandwidth_Hz,2,"Hz",value_string[0]),add_units(1.0/delta_time,2,"Hz",value_string[1]));
				}
			else
				{
				if (noise_location == ENTERED_NOISE_AMP_IS_UNFILTERED)
					{
					printf("Setting unfiltered noise_amp to %.4f\nfor noise bandwidth of %s and sampling frequency of %s.\n",
					unfiltered_noise_amp,add_units(noise_bandwidth_Hz,2,"Hz",value_string[0]),add_units(1.0/delta_time,2,"Hz",value_string[1]));
					}
				else
					{
					printf("Unknown noise injection location type, setting to after filter.\n");
					printf("Setting unfiltered_noise_amp to %.4f from noise_amp of %.4f\nfor noise bandwidth of %s and sampling frequency of %s.\n",
					unfiltered_noise_amp,noise_amp, add_units(noise_bandwidth_Hz,2,"Hz",value_string[0]),add_units(1.0/delta_time,2,"Hz",value_string[1]));
					}
				}
		#endif
		}
	
	if( (time_sec = (double *) calloc(num_points_per_period*(num_periods + 1 + settling_periods) + 1,sizeof(double))) == NULL)
		{
		printf("Error allocating memory to time_sec!\n");
		exit(0);
		}
	if( (vsin = (double *) calloc(num_points_per_period*(num_periods + 1 + settling_periods) + 1,sizeof(double))) == NULL)
		{
		printf("Error allocating memory to vsin!\n");
		exit(0);
		}
	if( (vsin_pm = (double *) calloc(num_points_per_period*(num_periods + 1 + settling_periods) + 1,sizeof(double))) == NULL)
		{
		printf("Error allocating memory to vsin_pm!\n");
		exit(0);
		}
	if( (vsq = (double *) calloc(num_points_per_period*(num_periods + 1 + settling_periods) + 1,sizeof(double))) == NULL)
		{
		printf("Error allocating memory to vsq!\n");
		exit(0);
		}
	if( (vout = (double *) calloc(num_points_per_period*(num_periods + 1 + settling_periods) + 1,sizeof(double))) == NULL)
		{
		printf("Error allocating memory to vout!\n");
		exit(0);
		}
	if( (vout_filtered = (double *) calloc(num_points_per_period*(num_periods + 1 + settling_periods) + 1,sizeof(double))) == NULL)
		{
		printf("Error allocating memory to vout_filtered!\n");
		exit(0);
		}
	if( (am_noise = (double *) calloc(num_points_per_period*(num_periods + 1 + settling_periods) + 1,sizeof(double))) == NULL)
		{
		printf("Error allocating memory to am_noise!\n");
		exit(0);
		}
	if( (pm_noise = (double *) calloc(num_points_per_period*(num_periods + 1 + settling_periods) + 1,sizeof(double))) == NULL)
		{
		printf("Error allocating memory to pm_noise!\n");
		exit(0);
		}
	if( (vth_cross_rise = (double *) calloc(2*(num_periods + settling_periods),sizeof(double))) == NULL)
		{
		printf("Error allocating memory to vth_cross_rise!\n");
		exit(0);
		}
	if( (vth_cross_fall = (double *) calloc(2*(num_periods + settling_periods),sizeof(double))) == NULL)
		{
		printf("Error allocating memory to vth_cross_fall!\n");
		exit(0);
		}
	if( (duty_cycle = (double *) calloc(2*(num_periods + settling_periods),sizeof(double))) == NULL)
		{
		printf("Error allocating memory to duty_cycle!\n");
		exit(0);
		}
		
	k = 0;
	end_flag = 0;
	
	if (noise_amp != 0.0)
		{
		#ifdef DEBUG_NOISE
			sprintf(pinput_string,"fnoise_%s.csv",ptimestamp);
			fp_noise = fopen(pinput_string,"w");
			fprintf(fp_noise,"Time (sec),noise (m%%),filtered noise (m%%)\n");
		#endif
		}
		
   for (i = 0; i < num_points_per_period*(num_periods + 1 + settling_periods) + 2; i++)
      {
		time_sec[i] = delta_time *((double) i);
	   /*Add random phase noise of maximum magnitude "noise_amp_pp" UIpp to fin*/
	   
	   if (noise_amp != 0.0)
	      {
	      switch (noise_type)
	         {
	         case GAUSSIAN_NOISE:
	            noise = random_gaussian_clocknoise(unfiltered_noise_amp/3.0,0.0,seed);
	            noise_vthreshold = noise_amp/2.0;
	            if(rkstep1(time_sec[i],time_sec[i] + delta_time,tau_noise_bandwidth_sec,
	            noise,&filtered_noise,10.0,-10.0,errmax,MAX_ITERATIONS) == 1)
            		{
            		printf("Error in runge-kutta routine for noise, program exits!\n");
           			 exit(0);
            		}
	            break;
	         case UNIFORM_NOISE:
	         	noise = random_uniform_clocknoise(2.0*unfiltered_noise_amp,0.0,seed);
	         	noise_vthreshold = noise_amp/2.0;
	            if(rkstep1(time_sec[i],time_sec[i] + delta_time,tau_noise_bandwidth_sec,
	            noise,&filtered_noise,10.0,-10.0,errmax,MAX_ITERATIONS) == 1)
            		{
            		printf("Error in runge-kutta routine for noise, program exits!\n");
           			 exit(0);
            		}
	            break;
	         case SINUSOIDAL_NOISE:
	         	noise = noise_amp*sin(2.0*pi*noise_bandwidth_Hz*time_sec[i]);
	         	noise_vthreshold = noise_amp;
	         	filtered_noise = noise;
	         	break;
	         default:
	            printf("In main.c, did NOT recognize noise type! Exiting...\n");
	            exit(0);
	         }
	      	      
	      if (filtered_noise > noise_max)
	         noise_max = filtered_noise;
	      if (filtered_noise < noise_min)
	         noise_min = filtered_noise;
	      if (fabs(filtered_noise - last_filtered_noise) > maximum_delta_noise)
	         maximum_delta_noise = filtered_noise - last_filtered_noise;
	      last_filtered_noise = filtered_noise;
	      if (fabs(filtered_noise) > noise_vthreshold)
	         noise_counter++;
	      #ifdef DEBUG_NOISE
	      	fprintf(fp_noise,"%1.12e,%1.12e,%1.12e\n",time_sec[i],noise/1e-03,filtered_noise/1e-03);
	      #endif
			switch (modulation_type)
				{
				case AM_MODULATION:
	      		am_noise[i] = filtered_noise;
	      		pm_noise[i] = 0.0;
	      		break;
	      	case PM_MODULATION:
	      		pm_noise[i] = filtered_noise;
	      		am_noise[i] = 0.0;
	 	      		break;
	      	default:
	      		printf("In main.c, did NOT recognize modulation type! Exiting...\n");
	            exit(0);
				}
	      	
	      }
      else
      	{
      	am_noise[i] = 0.0;
      	pm_noise[i] = 0.0;
      	} 	
     	}
 if (noise_amp != 0.0)
 	{    	
	#ifdef DEBUG_NOISE
		fclose(fp_noise);
	#endif
	}     	
/*---------------------------------------*/

length_vth_cross_rise = 0;
length_vth_cross_fall = 0;

for (i = 0;i < num_points_per_period*(num_periods + 1 + settling_periods) + 2;i++)
	{
	time_sec[i] = delta_time *((double) i);
	vsin[i] = sin(2.0*pi*time_sec[i]*freq_Hz + init_phase_rad);
	vsin_pm[i] = sin(2.0*pi*time_sec[i]*freq_Hz + pm_noise[i]*2.0*pi + init_phase_rad);
	if (vsin[i] >= vthreshold)
		{
		vsq[i] = 1.0;
		if (i > 0)
			{
			if ((vsin[i] >= vthreshold) && (vsin[i - 1] < vthreshold))
				{
				vth_cross_rise[length_vth_cross_rise] = time_sec[i] + pm_noise[i]*per_sec;
				length_vth_cross_rise++;
				}
			}
		else
			{
			vsq[i] = 1.0;
			}
		}
	else
		{
		vsq[i] = 0.0;
		if (i > 0)
			{
			if ((vsin[i] <= vthreshold) && (vsin[i - 1] > vthreshold))
				{
				vth_cross_fall[length_vth_cross_fall] = time_sec[i] + pm_noise[i]*per_sec;
				length_vth_cross_fall++;
				}
			}
			else
				{
				vsq[i] = 0.0;
				}
			}
		}

/* -------------------------------------------- */


if (length_vth_cross_fall >= length_vth_cross_rise)
	num_crossings = length_vth_cross_rise;
else
	num_crossings = length_vth_cross_fall;

if(vth_cross_fall[1] >= vth_cross_rise[1])
	{
	for(i = 1;i < num_crossings;i++)
		duty_cycle[i - 1] = 100.0*(vth_cross_fall[i] - vth_cross_rise[i])/(vth_cross_rise[i] - vth_cross_rise[i - 1]);
	}
else
	{
	for(i = 1;i < num_crossings;i++)
		duty_cycle[i - 1] = 100.0*(1.0 - (vth_cross_rise[i] - vth_cross_fall[i])/(vth_cross_rise[i] - vth_cross_rise[i - 1]));
	}
#ifdef DEBUG_DUTY_CYCLE
	FILE *fp_du;
	sprintf(pinput_string,"duty_cycle_%s.csv",ptimestamp);
	fp_du = fopen(pinput_string,"w");
	for (i = 0; i < (num_crossings -1);i++)
		fprintf(fp_du,"%ld,%.6f,%1.12e,%1.12e\n",i,duty_cycle[i],vth_cross_rise[i],vth_cross_fall[i]);
	fclose(fp_du);
#endif

mean_du = mean(duty_cycle,num_crossings - 1);

j = 0;
k = 0;

for (i = 0; i < num_points_per_period*(num_periods + 1 + settling_periods) + 2; i++)
	{
	if((time_sec[i] >= (vth_cross_rise[j] - ttran_rise/2.0)) && (time_sec[i] < (vth_cross_rise[j] + ttran_rise/2.0)))
		vout[i] = 0.50 + (time_sec[i] - vth_cross_rise[j])/ttran_rise;
	else
		{
		if((time_sec[i] >= (vth_cross_fall[k] - ttran_fall/2.0)) && (time_sec[i] < (vth_cross_fall[k] + ttran_fall/2.0)))
			 vout[i] = 0.50 + (vth_cross_fall[k] - time_sec[i])/ttran_fall;
		else
			{
			if (i > 0)
				{
				if (vout[i - 1] > 0.50)
					vout[i] = 1.0;
				else
					vout[i] = 0.0;
				}
			else
				vout[i] = vsq[0];
			}
		}
	if (((time_sec[i + 1] - vth_cross_rise[j]) >= ttran_rise/2.0) && (j < length_vth_cross_rise))
		{
		j++;
		/* printf("incrementing rise event at %1.4e. j = %ld\n",time_sec[i],j); */
		}
	if (((time_sec[i + 1] - vth_cross_fall[k]) >= ttran_fall/2.0) && (k < length_vth_cross_fall))
		{
		k++;
		/* printf("incrementing fall event at %1.4e. k = %ld\n",time_sec[i],k); */
		}
	}

/* Remove first period since its initial transition time was not set to ttran_rise nor ttran_fall */
/* Apply AM modulation to signals */
j = 0;
for (i = 0; i < num_points_per_period*(num_periods + 1 + settling_periods) + 2; i++)
	{
	if (i >= num_points_per_period)
		{
		time_sec[j] = time_sec[i] = delta_time *((double) j);
		vsin_pm[j] = vsin_pm[i]*(1.0 + am_noise[i]);
		vsq[j] = vsq[i]*(1.0 + am_noise[i]);
		vout[j] = vout[i]*(1.0 + am_noise[i]);
		if (j > 0)
			{
			if (tau_vout_sec != 0.0)
				{
				if(rkstep1(time_sec[j - 1],time_sec[j],tau_vout_sec,
	   		vout[j],&vout_filtered_cap,5,-5,errmax,MAX_ITERATIONS) == 1)
         		{
         		printf("Error in runge-kutta routine for RC filtered vout, program exits!\n");
         		exit(0);
         		}
         	}
         else
         	{
         	vout_filtered_cap = vout[j];
         	}
         }
      else
      	vout_filtered_cap = vout[j];
      vout_filtered[j] = vout_filtered_cap;
		j++;
		}
	}

for (i = j; i < num_points_per_period*(num_periods + 1 + settling_periods + 2);i++)
	{
	time_sec[i] = 0.0;
	vsin_pm[i] = 0.0;
	vsq[i] = 0.0;
	vout[i] = 0.0;
	vout_filtered[i] = 0.0;
	}

/* Remove settling periods */

j = num_points_per_period*settling_periods + 1;

for (i = j; i < num_points_per_period*(num_periods + settling_periods) + 2;i++)
	{
	time_sec[i - j] = delta_time * ((double) (i - j));
	vsin_pm[i - j] = vsin_pm[i];
	vsq[i - j] = vsq[i];
	vout[i - j] = vout[i];
	vout_filtered[i - j] = vout_filtered[i];
	}

for (l = i - j; l < num_points_per_period*(num_periods + 1 + settling_periods) + 2; l++)
	{
	time_sec[l] = 0.0;
	vsin_pm[l] = 0.0;
	vsq[l] = 0.0;
	vout[l] = 0.0;
	vout_filtered[l] = 0.0;
	}

#ifdef FILTER_VOUT	
for (i = 0; i < j + num_points_per_period;i++)
	{
	vout[i] = vout_filtered[i];
	}
#endif

/* Compute duty cycle of filtered waveform */

for (i = 0; i< length_vth_cross_rise; i++)
	vth_cross_rise[i] = 0.0;
for (i = 0; i< length_vth_cross_fall; i++)
	vth_cross_fall[i] = 0.0;
for (i = 0; i< num_crossings; i++)
	duty_cycle[i] = 0.0;
	
length_vth_cross_rise = 0;
length_vth_cross_fall = 0;

for (i = 1;i < num_points_per_period*num_periods + 1;i++)
	{
	if (vout_filtered[i] >= vout_threshold)
		{
		if ((vout_filtered[i] >= vout_threshold) && (vout_filtered[i - 1] < vout_threshold))
			{
			vth_cross_rise[length_vth_cross_rise] = time_sec[i];
			length_vth_cross_rise++;
			}
		}
	else
		{
		if ((vout_filtered[i] < vout_threshold) && (vout_filtered[i - 1] >= vout_threshold))
			{
			vth_cross_fall[length_vth_cross_fall] = time_sec[i];
			length_vth_cross_fall++;
			}
		}
		if (vout_filtered[i] > vout_filtered_max)
	      vout_filtered_max = vout_filtered[i];
	   if (vout_filtered[i] < vout_filtered_min)
	      vout_filtered_min = vout_filtered[i];
	}

/* -------------------------------------------- */

if ((length_vth_cross_fall != 0) && (length_vth_cross_rise != 0))
	{
	if (length_vth_cross_fall >= length_vth_cross_rise)
		num_crossings = length_vth_cross_rise;
	else
		num_crossings = length_vth_cross_fall;
	
	if(vth_cross_fall[1] >= vth_cross_rise[1])
		{
		for(i = 1;i < num_crossings;i++)
			duty_cycle[i - 1] = 100.0*(vth_cross_fall[i] - vth_cross_rise[i])/(vth_cross_rise[i] - vth_cross_rise[i - 1]);
		}
	else
		{
		for(i = 1;i < num_crossings;i++)
			duty_cycle[i - 1] = 100.0*(1.0 - (vth_cross_rise[i] - vth_cross_fall[i])/(vth_cross_rise[i] - vth_cross_rise[i - 1]));
		}
	mean_vout_filtered_du = mean(duty_cycle,num_crossings - 1);
	printf("Computed mean duty cycle of filtered output of %.2f%% (+/- %.2f%%) from %ld threshold crossings.\n",mean_vout_filtered_du,100.0*delta_time*freq_Hz,num_crossings);
	}
else
	{
	num_crossings = 0;
	mean_vout_filtered_du = 0.0;
	if (vout_filtered_max < vout_threshold)
		printf("Filtered output waveform has a maximum of %s and does not cross threshold of %s.\n",
		add_units(vout_filtered_max,1,"V",value_string[1]),add_units(vout_threshold,1,"V",value_string[0]));
	else
		printf("Filtered output waveform has a minimum of %s and does not cross threshold of %s.\n",
		add_units(vout_filtered_min,1,"V",value_string[1]),add_units(vout_threshold,1,"V",value_string[0]));
	}

/* End of compute duty cycle of filtered waveform */

/*---------------------------------------*/

      
/*Open output file and print header to file fpw1*/

fpw1 = fopen(pfnameout,"w");
#ifdef DEBUG_SQUARE_WAVE
	if (tau_vout_sec != 0.0)
		{
   	fprintf(fpw1,"Time (sec),vsin (V),vsin_pm (V),vsq (V),vout (V),vout filtered (V) tau = %s (%s)\n",add_units(tau_vout_sec,3,"s",value_string[0]),add_units(1.0/(2.0*pi*tau_vout_sec),2,"Hz",value_string[1]));
   	}
   else
		{
		fprintf(fpw1,"Time (sec),vsin (V),vsin_pm (V),vsq (V),vout (V),vout filtered (V) tau = infinite (no bandlimiting)\n");
		}
#else
	if (tau_vout_sec != 0.0)
		{
		fprintf(fpw1,"Time (s),vout filtered (V) tau = %s (%s)\n",add_units(tau_vout_sec,3,"s",value_string[0]),add_units(1.0/(2.0*pi*tau_vout_sec),3,"Hz",value_string[1]));
   	}
   else
		{
		fprintf(fpw1,"Time (s),vout (no bandlimiting)\n");
		}	   
#endif

/*Set counter to determine number of printed points (time_point_counter) */

time_point_counter = 0;

for (i = 0; i < num_points_per_period*num_periods; i++)
	{

   /*Print data to file if time > TSTART and time is close to printstep*/

   prtime = ((double) time_point_counter)*prpoint + TSTART;
   if (((prtime - time_sec[i]) < delta_time) && (fabs(prtime - time_sec[i]) <= fabs(prtime - time_sec[i + 1])))
      {
      #ifdef DEBUG_SQUARE_WAVE
   		sprintf(pline1,"%1.12e,%1.12e,%1.12e,%1.12e,%1.12e,%1.12e\n",time_sec[i],vsin[i],vsin_pm[i],vsq[i],vout[i],vout_filtered[i]);
      #else
      	sprintf(pline1,"%1.12e,%1.12e\n",time_sec[i],vout_filtered[i]);
      #endif
		fprintf(fpw1,"%s",pline1);
      ++time_point_counter;
      }
   }
fclose(fpw1);

free(time_sec);
free(vsin);
free(vsin_pm);
free(vsq);
free(vout);
free(vout_filtered);
free(am_noise);
free(pm_noise);
free(vth_cross_rise);
free(vth_cross_fall);
free(duty_cycle);
 
if (noise_amp != 0.0)
   {
   printf("Input Data: noise_max = %.4f m%%, noise_min = %.2f m%%, or %.2f m%%pp.\n",
   noise_max/1e-03, noise_min/1e-03, (noise_max - noise_min)/1e-03);
   printf("Maximum change in noise over 1 timestep = %.4f m%%\n",maximum_delta_noise/1e-03);
   if (noise_type != SINUSOIDAL_NOISE)
   	printf("The magnitude of the noise exceeded %.2f m%% a total of %ld times.\n",
   noise_vthreshold/1e-03,noise_counter);
   }
   
/* Create command lines for plotting waveform in gnuplot and plot if gnuplot is available */

if ((check_executable(pgnuplot,pgnuplot_path)) == 0)
	{
	printf("Did not locate executable for gnuplot in $PATH.\n");
	printf("Disabling all plots.\n");
	}
else
	{
	/* Generate text file with gnuplot output command */
	if (noise_amp != 0.0)
		{
		if (noise_type != SINUSOIDAL_NOISE)
			{
			if ( noise_type == GAUSSIAN_NOISE)
				{
				sprintf(ptitle_string,"{/:Bold %s Square Wave (Du = %.1f%%, tr = %.1f%%, tf = %.1f%%) %s Modulated by Gaussian Random Noise}\n{/:Bold modulation index (3 sigma) = %s, noise bandwidth = %s, sampling frequency = %s}",add_units(freq_Hz,2,"Hz",value_string[0]),duty_cycle_percent,ttran_rise_percent,ttran_fall_percent,pmodulation_string,add_units(noise_amp,1,"",value_string[1]),add_units(noise_bandwidth_Hz,2,"Hz",value_string[2]),add_units(1.0/delta_time,2,"Hz",value_string[3]));
				}
			else
				{
				sprintf(ptitle_string,"{/:Bold %s Square Wave (Du = %.1f%%, tr = %.1f%%, tf = %.1f%%) %s Modulated by Uniform Random Noise}\n{/:Bold modulation index (range/2) = %s, noise bandwidth = %s, sampling frequency = %s}",add_units(freq_Hz,2,"Hz",value_string[0]),duty_cycle_percent,ttran_rise_percent,ttran_fall_percent,pmodulation_string,add_units(noise_amp,1,"",value_string[1]),add_units(noise_bandwidth_Hz,2,"Hz",value_string[2]),add_units(1.0/delta_time,2,"Hz",value_string[3]));
				}
			}
		else
			sprintf(ptitle_string,"{/:Bold %s Square Wave (Du = %.1f%%, tr = %.1f%%, tf = %.1f%%) %s Modulated by Sinusoidal Signal}\n{/:Bold modulation index = %s, modulation frequency = %s, sampling frequency = %s}",add_units(freq_Hz,2,"Hz",value_string[0]),duty_cycle_percent,ttran_rise_percent,ttran_fall_percent,pmodulation_string,add_units(noise_amp,1,"",value_string[1]),add_units(noise_bandwidth_Hz,2,"Hz",value_string[2]),add_units(1.0/delta_time,2,"Hz",value_string[3]));
		}
	else
		sprintf(ptitle_string,"{/:Bold %s Square Wave (Du = %.1f%%, tr = %.1f%%, tf = %.1f%%)}\n{/:Bold sampling frequency = %s}",add_units(freq_Hz,2,"Hz",value_string[0]),duty_cycle_percent,ttran_rise_percent,ttran_fall_percent,add_units(1.0/delta_time,2,"Hz",value_string[3]));
	
	sprintf(pinput_string,"head -1 %s > ./.%s\n",pfnameout,pfnameout);
	system(pinput_string);
	sprintf(pinput_string,"tail -%ld %s >> ./.%s\n",5*num_points_per_period,pfnameout,pfnameout);
	system(pinput_string);
	
	#ifdef DEBUG_SQUARE_WAVE
		sprintf(poctave_command_1,"gnuplot -e 'base_filename = \"%s\"; plot_title = \"%s\"; timestamp = \"%s\";' %s/plotting_routines/gnuplot/gnu_plot_debug.gnu \n",pbase_filename,ptitle_string,ptimestamp,PLOTTING_ROUTINES_DIR);
	#else
		sprintf(poctave_command_1,"gnuplot -e 'base_filename = \"%s\"; plot_title = \"%s\"; timestamp = \"%s\";' %s/plotting_routines/gnuplot/gnu_plot.gnu \n",pbase_filename,ptitle_string,ptimestamp,PLOTTING_ROUTINES_DIR);
	#endif

	system(poctave_command_1);
	sprintf(ptemp_string,"rm ./.%s\n",pfnameout);
	system(ptemp_string);
	
	if ((check_executable(pgimp,pgimp_path)) != 0)
		{
		sprintf(poctave_command_2,"gimp %s.png&\n",pbase_filename);
		system(poctave_command_2);
		}
	
/*		sprintf(pinput_string,"gnuplot_command_%s.txt",ptimestamp);
	fpw1 = fopen(pinput_string,"w");
	fprintf(fpw1,"%s",poctave_command_1);
	fclose(fpw1); */
	
	}

if ((check_executable(ppsd_sppowr,ppsd_sppowr_path)) == 0)
	{
	printf("Did not locate executable for psd_sppowr in $PATH.\n");
	printf("Disabling power spectral density analysis and plot.\n");
	}
else
	{
#ifdef DEBUG_SQUARE_WAVE
	sprintf(pinput_string,"psd_sppowr %s 1 6 4 0 7\n",pfnameout);
#else
	sprintf(pinput_string,"psd_sppowr %s 1 2 4 0 7\n",pfnameout);
#endif
	system(pinput_string);
	}

#ifdef COMPUTE_JITTER

if (num_crossings > MINIMUM_NUMBER_THRESHOLD_CROSSINGS)
	{
	if ((check_executable(pjitterhistv16,pjitterhistv16_path)) == 0)
		{
		printf("Did not locate executable for jitterhistv16 in $PATH.\n");
		printf("Disabling all jitter computations.\n");
		}
	else
		{
		/* Create jitter file name, remove 3 digit extension and */
		/* append "_jitter" and replace extension */
		
		if (append_filename_keep_N_characters(pfnameout,pfnameout_jitter,"_jitter",strlen(ptimestamp) + 5,LINELENGTH + 1) != EXIT_SUCCESS)
			{
			printf("Error in created corrected filename from function append_filename_keep_extension()\n");
			exit(0);
			}
		if ((duty_cycle_percent > 89.0) || (duty_cycle_percent < 11.0))
			num_samples_moving_average = 0;
	#ifdef DEBUG_SQUARE_WAVE
			sprintf(pinput_string,"jitterhistv16 %s 6 %s %.4e 0.50 %d y n %.4e\n",
			pfnameout,pfnameout_jitter,1e-09/delta_time,num_samples_moving_average,1e-06*freq_Hz);
			system(pinput_string);
	#else
			sprintf(pinput_string,"jitterhistv16 %s 2 %s %.4e 0.50 %d y n %.4e\n",
			pfnameout,pfnameout_jitter,1e-09/delta_time,num_samples_moving_average,1e-06*freq_Hz);
			system(pinput_string);
	#endif
		}
	}
else
	printf("Too few threshold crossings (%ld) to accurately estimate jitter. No jitter analysis performed.\n",
	num_crossings);
#endif

if (num_points_per_period*num_periods > 10000)
	{
	sprintf(pinput_string,"gzip %s\n",pfnameout);
	system(pinput_string);
	}


return EXIT_SUCCESS;
}

