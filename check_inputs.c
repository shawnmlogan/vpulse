#include "vpulse.h"

int check_inputs(double freq_Hz,double ttran_rise_percent,double ttran_fall_percent,double duty_cycle_percent, double vout_bandwidth_multiplier, double noise_amp, double noise_bandwidth_Hz, long int num_points_per_period, long int num_periods_to_plot, long int num_periods, char * pnoise_type_string, int *noise_type, char *pnoise_location_string, int *noise_location, char * pmodulation_type_string, int *modulation_type, double init_phase_degrees, double *init_phase_rad)
{
int return_status = EXIT_SUCCESS;
char value_string[NUMBER_OF_VALUE_STRINGS][LINELENGTH_OF_VALUE_STRING + 1];

   if (freq_Hz <= 0.0)
   	{
   	printf("Enter non-zero and positive frequency in Hz, read value of %1.6e.\n",freq_Hz);
   	return_status = EXIT_FAILURE;
   	}

*init_phase_rad = init_phase_degrees*pi/180.0;
		
if ((ttran_rise_percent <= 0.0) || (ttran_rise_percent >= 50.0))
   	{
   	printf("Enter rise time greater than 0 %% and less than 50%% of period, read value of %1.2e.\n",ttran_rise_percent);
   	return_status = EXIT_FAILURE;
   	}
   	
if ((ttran_fall_percent <= 0.0) || (ttran_fall_percent >= 50.0))
   	{
   	printf("Enter fall time greater than 0 %% and less than 50%% of period, read value of %1.2e.\n",ttran_fall_percent);
   	return_status = EXIT_FAILURE;
   	}

if (((ttran_rise_percent * (double) num_points_per_period/100.0) < MIN_NUM_POINTS_PER_TRANSIION) || ((ttran_fall_percent * (double) num_points_per_period/100.0) < MIN_NUM_POINTS_PER_TRANSIION))
   	{
   	if (ttran_rise_percent <= ttran_fall_percent)
   		{
   		printf("Insufficient number of points per waveforms specified for rise transition time of %s.\n",
   		add_units(ttran_rise_percent,1,"%",value_string[0]));
   		printf("Enter at least %.0f number of points per period.\n",
   		ceil(100.0* (double) MIN_NUM_POINTS_PER_TRANSIION/ttran_rise_percent));
   		}
   	else
   		{
   		printf("Insufficient number of points per waveforms specified for fall transition time of %s .\n",
   		add_units(ttran_fall_percent,1,"%",value_string[0]));
   		printf("Enter at least %.0f number of points per period.\n",
   		ceil(100.0* (double) MIN_NUM_POINTS_PER_TRANSIION/ttran_fall_percent));
   		}
   	return_status = EXIT_FAILURE;
   	}

if (duty_cycle_percent < 0.0)
   	{
   	printf("Enter a positive duty cycle., read value of %.2f.\n",duty_cycle_percent);
   	return_status = EXIT_FAILURE;
   	}
   	
if ((duty_cycle_percent * (double) num_points_per_period/100.0) < MIN_NUM_POINTS_PER_ONTIME)
   	{
   	printf("Insufficient number of points per waveforms specified for duty cycle of %s.\n",
   	add_units(duty_cycle_percent,1,"%",value_string[0]));
   	printf("Enter at least %.0f number of points per period.\n",
   	ceil(100.0* (double) MIN_NUM_POINTS_PER_ONTIME/duty_cycle_percent));
   	}

if ((duty_cycle_percent + ttran_rise_percent + ttran_fall_percent) > 100.0)
   	{
   	printf("Sum of rise time (%.2f), fall time (%.2f), and duty cycle (%.2f) exceeds 100%%.\n",
   	ttran_rise_percent,ttran_fall_percent,duty_cycle_percent);
   	return_status = EXIT_FAILURE;
   	}

if ((duty_cycle_percent - (ttran_rise_percent + ttran_fall_percent)) < 0.0)
   	{
   	printf("Rise time (%.2f) and fall time (%.2f) and duty cycle (%.2f) will prevent waveform from achieving logic high.\n",
   	ttran_rise_percent,ttran_fall_percent,duty_cycle_percent);
   	return_status = EXIT_FAILURE;
   	}

if (((100.0 - duty_cycle_percent) - (ttran_rise_percent + ttran_fall_percent)) < 0.0)
   	{
   	printf("Rise time (%.2f) and fall time (%.2f) and duty cycle (%.2f) will prevent waveform from achieving logic low.\n",
   	ttran_rise_percent,ttran_fall_percent,duty_cycle_percent);
   	return_status = EXIT_FAILURE;
   	}

if ((vout_bandwidth_multiplier < 0.0) || (vout_bandwidth_multiplier > MAXIMUM_BANDWIDTH_MULTIPLIER))
	{
	printf("Entered value of %.2f for output square wave RC filter bandwidth must be",vout_bandwidth_multiplier);
	printf(" greater or equal to 0 and less than or equal to %.0f).\n",MAXIMUM_BANDWIDTH_MULTIPLIER);
	return_status = EXIT_FAILURE;
	}
	
if (num_points_per_period <= 1)
   {
	printf("Enter more than 1 number of points per period, read value of %ld.\n",num_points_per_period);
	return_status = EXIT_FAILURE;
	}

if ((num_periods_to_plot > num_periods) || (num_periods_to_plot < 1))
	{
	printf("Number of periods to plot must be greater than 1 and less than or equal to num_periods (%ld).\n",num_periods);
	return_status = EXIT_FAILURE;
	}

if (noise_amp != 0.0)
	{
   if (fabs(noise_amp) > 1.0)
   	{
   	printf("Enter a modulation index (noise_amp) of less than 1.0, read value of %1.6e.\n",noise_amp);
   	return_status = EXIT_FAILURE;
   	}

   if (noise_bandwidth_Hz <= 0.0)
   	{
   	printf("Enter non-zero and positive noise bandwidth in Hz, read value of %1.6e.\n",noise_bandwidth_Hz);
   	return_status = EXIT_FAILURE;
   	}
		
	if (find_modulation_type(pmodulation_type_string,modulation_type) != EXIT_SUCCESS)
		{
		printf("Could not identify modulation type entry of \"%s\" as either AM or PM\n",pmodulation_type_string);
		printf("Please re-enter AM (a) or PM (p) (case insensitive).\n");
		return_status = EXIT_FAILURE;
		}

	if (find_noise_type(pnoise_type_string,noise_type) != EXIT_SUCCESS)
		{
		printf("Could not identify noise type entry of \"%s\" as either gaussian, uniform, or sinusoidal n",pnoise_type_string);
		printf("Please re-enter gaussian (g), uniform (u), or sinusoidal (s) (case insensitive).\n");
		return_status = EXIT_FAILURE;
		}

	if (find_noise_location(pnoise_location_string,noise_location) != EXIT_SUCCESS)
		{
		printf("Could not identify noise type entry of \"%s\" as either gaussian, uniform, or sinusoidal n",pnoise_type_string);
		printf("Please re-enter gaussian (g), uniform (u), or sinusoidal (s) (case insensitive).\n");
		return_status = EXIT_FAILURE;
		}
	}
	
return return_status;	

}