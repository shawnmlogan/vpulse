#define MAIN

#include "vpulse.h"

int main(int argc, char **argv)
{

/*Define variables*/

double *time_sec, *vpulse, *time_sq, *vout_sq, *vout, prpoint, prtime, TSTART = 0.0;
double tau=EPSILON, tau_noise_bandwidth_sec;
double noise = 0.0, noise_amp = 0.10, noise_bandwidth_Hz;
double noise_vthreshold, filtered_noise = 0.0, noise_max = -999.0, noise_min =999.0;
double freq_Hz, per_sec, ttran_percent, ttran, duty_cycle_percent, pulse_start_time, pulse_end_time, delta_time;
double errmax = 100.0;

int noise_type = UNIFORM_NOISE;

unsigned int seed;

long int i, j, k;
long int num_points_per_period, num_periods, num_periods_to_plot; 
long int noise_counter = 0, time_point_counter, loopcount;

char fnameout[LINELENGTH + 1], *pfnameout;
char line1[LINELENGTH+1], *pline1, *pinput_string, input_string[LINELENGTH + 1];

char noise_type_string[LINELENGTH+1];
char *ptimestamp,timestamp[LINELENGTH + 1];

char fileout_octave_command_line[LINELENGTH+1], *pfileout_octave_command_line;

char octave_command_1[LINELENGTH + 1], octave_command_2[LINELENGTH + 1], octave_command_3[LINELENGTH + 1];
char *poctave_command_1, *poctave_command_2, *poctave_command_3;

char value_string[NUMBER_OF_VALUE_STRINGS][LINELENGTH_OF_VALUE_STRING + 1];
char *ptitle_string, title_string[LINELENGTH + 1];
char *poctave = "octave",*poctave_path,octave_path[LINELENGTH + 1];
char *pgnuplot = "gnuplot",*pgnuplot_path,gnuplot_path[LINELENGTH + 1];
char plot_preference[LINELENGTH + 1];


FILE *fpw1;

/*Define pi */

pi=2.0 * asin(1.0);

/*Initialize pointers*/

pfnameout = &fnameout[0];
pfileout_octave_command_line = &fileout_octave_command_line[0];
pline1 = &line1[0];
ptimestamp = &timestamp[0];
pinput_string = &input_string[0];

poctave_command_1 = &octave_command_1[0];
poctave_command_2 = &octave_command_2[0];
poctave_command_3 = &octave_command_3[0];
ptitle_string = &title_string[0];
poctave_path = &octave_path[0];
pgnuplot_path = &gnuplot_path[0];


printf("\nvpulse v%.2f %s\n\n",VERSION_NUMBER,VERSION_DATE);

if (argc != 9)
	{
	printf("Usage with command line:\nvpulse <input_square_wave_freq_hz> <trf_per_cent_period> <duty_cycle_percent> <num_points_per_period> <num_periods>\n");

	printf("Enter square wave freq (Hz):\n");
	fgets(pinput_string,LINELENGTH,stdin);
	remove_carriage_return(pinput_string);
	freq_Hz = atof(pinput_string);	
	
	printf("Enter rise and fall time (percent of period):\n");
	fgets(pinput_string,LINELENGTH,stdin);
	remove_carriage_return(pinput_string);
	ttran_percent = atof(pinput_string);

	printf("Enter duty cycle (percent of period):\n");
	fgets(pinput_string,LINELENGTH,stdin);
	remove_carriage_return(pinput_string);
	duty_cycle_percent = atol(pinput_string);

	printf("Enter number of points per period:\n");
	fgets(pinput_string,LINELENGTH,stdin);
	remove_carriage_return(pinput_string);
	num_points_per_period = atol(pinput_string);

	printf("Enter number of periods:\n");
	fgets(pinput_string,LINELENGTH,stdin);
	remove_carriage_return(pinput_string);
	num_periods = atol(pinput_string);

	printf("Enter noise amplitude (V):\n");
	fgets(pinput_string,LINELENGTH,stdin);
	remove_carriage_return(pinput_string);
	noise_amp = atof(pinput_string);

	printf("Enter noise bandwidth (Hz):\n");
	fgets(pinput_string,LINELENGTH,stdin);
	remove_carriage_return(pinput_string);
	noise_bandwidth_Hz = atof(pinput_string);
	
	printf("Enter the number of periods to plot:\n");
	fgets(pinput_string,LINELENGTH,stdin);
	remove_carriage_return(pinput_string);
	num_periods_to_plot = atol(pinput_string);

	}
else
	{
	strncpy(pinput_string,argv[1],LINELENGTH);
	freq_Hz = atof(pinput_string);
	
	strncpy(pinput_string,argv[2],LINELENGTH);
	ttran_percent = atof(pinput_string);

	strncpy(pinput_string,argv[3],LINELENGTH);
	duty_cycle_percent = atof(pinput_string);
	
	strncpy(pinput_string,argv[4],LINELENGTH);
	num_points_per_period = atol(pinput_string);

	strncpy(pinput_string,argv[5],LINELENGTH);
	num_periods  = atol(pinput_string);
	
	strncpy(pinput_string,argv[6],LINELENGTH);
	noise_amp = atof(pinput_string);

	strncpy(pinput_string,argv[7],LINELENGTH);
	noise_bandwidth_Hz = atof(pinput_string);

	strncpy(pinput_string,argv[8],LINELENGTH);
	num_periods_to_plot = atol(pinput_string);

	}
	
	if ((num_periods_to_plot > num_periods) || (num_periods_to_plot < 1))
		{
		printf("Number of periods to plot must be greater than 1 and less than or equal to num_periods (%ld)..exiting\n",num_periods);
		exit(0);
		}

printf("Analysis parameters:\n");
printf("Square wave input frequency = %1.6e Hz\n",freq_Hz);
printf("Rise/fall time = %.1f %% of period\n",ttran_percent);
printf("Duty cycle = %.1f %% of period\n",duty_cycle_percent);
printf("num_points_per_period = %ld\n",num_points_per_period);
printf("num_periods = %ld\n",num_periods);
printf("noise_amp = %.2f mV\n",noise_amp/1e-3);
printf("noise_bandwidth = %s\n",add_units(noise_bandwidth_Hz,2,"Hz",value_string[0]));
printf("num_periods_to_plot = %ld\n",num_periods_to_plot);

	/* Find timestamp to append to filenames */

	find_timestamp(ptimestamp,LINELENGTH);
	sprintf(pfnameout,"square_wave_%.0fmeg_ttran_%.0f_percent_du_%.0f_noise_amp_%.0fm_noise_bw_%.0fmeg_%s.csv",freq_Hz/1e6,ttran_percent,duty_cycle_percent,noise_amp/1e-03,noise_bandwidth_Hz/1e6,ptimestamp);
	printf("Output filename is \"%s\".\n",pfnameout);

   /*Set counter to determine number of printed points (time_point_counter), initialize user notification counter k*/

   time_point_counter = 0;
   loopcount=1;
   k = 0;

   /*Print header to file fpw1*/

   fpw1 = fopen(pfnameout,"w");
	fprintf(fpw1,"Time(s),vout_sq (V),vout (V)\n");
         
   /*Start loop*/
   
   freq_Hz = 100e6;
	per_sec = 1.0/freq_Hz;
	ttran = (((double) ttran_percent + 0.0)/100.0)*per_sec;
	pulse_start_time = ((100.0 - duty_cycle_percent)/100.0)*per_sec - ttran;
	pulse_end_time = per_sec - ttran;
	
	tau_noise_bandwidth_sec = 1.0/(2*pi*noise_bandwidth_Hz);
	
	delta_time = per_sec/((double) num_points_per_period - 1);
	prpoint = delta_time;
	
	TSTART = delta_time*( (double) (num_points_per_period*(num_periods - num_periods_to_plot)));
	printf("Analysis is for %s, data printed starting at %s.\n",add_units(delta_time*( (double) num_points_per_period*num_periods),3,"s",value_string[0]),add_units(TSTART,3,"s",value_string[1]));
	
	if( (time_sec = (double *) calloc(num_points_per_period,sizeof(double))) == NULL)
		{
		printf("Error allocating memory to time_sec!\n");
		exit(0);
		}
	if( (vpulse= (double *) calloc(num_points_per_period,sizeof(double))) == NULL)
		{
		printf("Error allocating memory to vpulse!\n");
		exit(0);
		}
	if( (time_sq = (double *) calloc(num_points_per_period*num_periods,sizeof(double))) == NULL)
		{
		printf("Error allocating memory to time!\n");
		exit(0);
		}
	if( (vout_sq = (double *) calloc(num_points_per_period*num_periods,sizeof(double))) == NULL)
		{
		printf("Error allocating memory to vout_sq!\n");
		exit(0);
		}
	if( (vout = (double *) calloc(num_points_per_period*num_periods,sizeof(double))) == NULL)
		{
		printf("Error allocating memory to vout!\n");
		exit(0);
		}
	
	for(i = 0; i < (num_points_per_period - 1); i++)
		time_sec[i] = delta_time*((double) i);
	
	for (i = 0;i < num_points_per_period;i++)
		{
		if(time_sec[i] < pulse_start_time)
			vpulse[i] = 0.0;
		else
			{
			if(time_sec[i] < pulse_end_time)
				{
				if(vpulse[i-1] < 1.0)
					{
					if ((time_sec[i]-pulse_start_time)/ttran > 1.0)
						vpulse[i] = 1.0;
					else
						vpulse[i] = (time_sec[i]-pulse_start_time)/ttran;
					}
				else
					vpulse[i] = 1.0;
				}
			else
				{
				if(vpulse[i-1] >= 0.01)
					{
					if (((1.0 - (time_sec[i]-pulse_end_time)/ttran)) < 0.0)
						vpulse[i] = 0.0;
					else
						vpulse[i] = (1.0 - (time_sec[i]-pulse_end_time)/ttran);
					}
				else
					vpulse[i] = 0.0;
				}
			}
		}
		
	j = 0;
	
	for (i = 0; i < num_points_per_period*num_periods; i++)
		{
		if (j > (long int) floor(per_sec/delta_time))
			j = 0;
		vout_sq[i] = vpulse[j];
		time_sq[i] = delta_time*((double) i);
		j++;
		}
/*	for(i = 0;i<9;i++)
		printf("time_sq[%ld] = %1.6e, vout_sq[%ld] = %1.6e\n",i,time_sq[i],i,vout_sq[i]);*/
		
	noise_vthreshold = 2.0*noise_amp;
	
   for (i = 0; i < num_points_per_period*num_periods; i++)
      {

	   /*Add random phase noise of maximum magnitude "noise_amp_pp" UIpp to fin*/
	   
	   if (noise_amp != 0.0)
	      {
	      switch (noise_type)
	         {
	         case GAUSSIAN_NOISE:
	            noise = random_gaussian_clocknoise(noise_amp,0.0,seed);
	            break;
	         case UNIFORM_NOISE:
	         	noise = random_uniform_clocknoise(noise_amp,0.0,seed);
	            break;
	         default:
	            printf("In main.c, did NOT recognize noise type! Exiting...\n");
	            exit(0);
	         }
	      
	      if(rkstep1(time_sq[i],time_sq[i + 1],tau_noise_bandwidth_sec,noise,&filtered_noise,10.0,
         -10.0,errmax,MAX_ITERATIONS) == 1)
            {
            printf("Error in runge-kutta routine for noise, program exits!\n");
            exit(0);
            } 
	      
	      if (filtered_noise > noise_max)
	         noise_max = filtered_noise;
	      if (filtered_noise < noise_min)
	         noise_min = filtered_noise;
	      if (fabs(filtered_noise) > noise_vthreshold)
	         noise_counter++;

	      vout[i] = vout_sq[i]*filtered_noise; 
	      }
      else
      	vout[i] = vout_sq[i];

	   /*Print data to file if time > TSTART and time is close to printstep*/
	
      prtime = ((double) time_point_counter)*prpoint + TSTART;
      /*if (i < 10)
      	printf("prtime = %1.12e, time_sq[%ld] = %1.12e\n",prtime,i,time_sq[i]);*/

      if (((prtime - time_sq[i]) < delta_time) && (fabs(prtime - time_sq[i]) < fabs(prtime - time_sq[i + 1])))
         {
         sprintf(pline1,"%1.12e,%1.12e,%1.12e\n",time_sq[i],vout_sq[i],vout[i]);
			fprintf(fpw1,"%s",pline1);
         ++time_point_counter;
      	}
     } 
 
	   if (noise_amp != 0.0)
	      {
	      printf("Input Data: noise_max = %.4f mV, noise_min = %.2f mV, or %.2f mVpp.\n",
	      noise_max/1e-03, noise_min/1e-03, (noise_max - noise_min)/1e-03);
	      printf("The magnitude of the noise exceeded %.2f mV a total of %ld times.\n",
	      noise_vthreshold/1e-03,noise_counter);
	      }
	   
/* Only produce transient plot if there is no applied jitter frequency and the jitter amplitude is 0 */

fclose(fpw1);

sprintf(ptitle_string,"Input %s Square Wave (Du = %.1f %%, trf = %.1f %% of period) and Modulated Noise\nnoise\\_amp = %s, noise bandwidth = %s, sampling frequency = %s",add_units(freq_Hz,2,"Hz",value_string[0]),duty_cycle_percent,ttran_percent,add_units(noise_amp,2,"V",value_string[1]),add_units(noise_bandwidth_Hz,2,"Hz",value_string[2]),add_units(1.0/delta_time,2,"Hz",value_string[3]));

sprintf(poctave_command_1,"gnuplot -c ./gnu_plot.gnu \'%s\' \'%s\' \'%s\'\n",pfnameout,ptitle_string,ptimestamp);

sprintf(poctave_command_2,"open -a \"graphicconverter 12\" sq_%s.png\n",ptimestamp);

system(poctave_command_1);
system(poctave_command_2);
sprintf(pinput_string,"gnuplot_comamnd_%s.txt",ptimestamp);
fpw1 = fopen(pinput_string,"w");
fprintf(fpw1,"%s",poctave_command_1);
fclose(fpw1);

free(time_sec);
free(vpulse);
free(time_sq);
free(vout_sq);
free(vout);

return EXIT_SUCCESS;
}

