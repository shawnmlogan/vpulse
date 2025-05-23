#include "vpulse.h"

int check_vpulse_inputs(double freq_Hz,double ttran_rise_percent,double ttran_fall_percent,double duty_cycle_percent, double vout_bandwidth_multiplier, double noise_amp, double noise_bandwidth_Hz, long int num_points_per_period, long int num_periods_to_plot, long int num_periods, char * pnoise_type_string, int *noise_type, char *pnoise_location_string, int *noise_location, char * pmodulation_type_string, int *modulation_type, double init_phase_degrees, double *init_phase_rad, char *pyes_no_psd_string, int *pperform_psd_analysis_flag, char *pyes_no_tie_string, int *pperform_tie_analysis_flag)
{
int return_status = EXIT_SUCCESS, yes_no_value = 0;
char value_string[NUMBER_OF_VALUE_STRINGS][LINELENGTH_OF_VALUE_STRING + 1];

   if (freq_Hz <= 0.0)
   	{
   	printf("Enter non-zero and positive frequency in Hz, read value of %1.6e.\n",freq_Hz);
   	return_status = EXIT_FAILURE;
   	}

*init_phase_rad = init_phase_degrees*pi/180.0;
		
if (ttran_rise_percent < 0.0)
   	{
   	printf("Enter rise time greater than or equal to 0 %%, read value of %1.2e.\n",ttran_rise_percent);
   	return_status = EXIT_FAILURE;
   	}
   	
if (ttran_fall_percent < 0.0)
   	{
   	printf("Enter fall time greater than or equal to 0 %%, read value of %1.2e.\n",ttran_fall_percent);
   	return_status = EXIT_FAILURE;
   	}

if ((ttran_fall_percent > 0.0) &&  (ttran_rise_percent > 0.0))
	{
if (((ttran_rise_percent * (double) num_points_per_period/100.0) < MIN_NUM_POINTS_PER_TRANSIIION) || ((ttran_fall_percent * (double) num_points_per_period/100.0) < MIN_NUM_POINTS_PER_TRANSIIION))
   	{
   	if (ttran_rise_percent <= ttran_fall_percent)
   		{
   		printf("Insufficient number of points per waveforms specified for rise transition time of %s.\n",
   		add_units_2(ttran_rise_percent,1,0,0,"%",value_string[0]));
   		printf("Enter at least %.0f number of points per period.\n",
   		ceil(100.0* (double) MIN_NUM_POINTS_PER_TRANSIIION/ttran_rise_percent));
   		}
   	else
   		{
   		printf("Insufficient number of points per waveforms specified for fall transition time of %s .\n",
   		add_units_2(ttran_fall_percent,1,0,0,"%",value_string[0]));
   		printf("Enter at least %.0f number of points per period.\n",
   		ceil(100.0* (double) MIN_NUM_POINTS_PER_TRANSIIION/ttran_fall_percent));
   		}
   	return_status = EXIT_FAILURE;
   	}
   }

if (duty_cycle_percent < 0.0)
   	{
   	printf("Enter a positive duty cycle., read value of %.2f.\n",duty_cycle_percent);
   	return_status = EXIT_FAILURE;
   	}
   	
if ((duty_cycle_percent * (double) num_points_per_period/100.0) < MIN_NUM_POINTS_PER_ONTIME)
   	{
   	printf("Insufficient number of points per waveforms specified for duty cycle of %s.\n",
   	add_units_2(duty_cycle_percent,1,0,0,"%",value_string[0]));
   	printf("Enter at least %.0f number of points per period.\n",
   	ceil(100.0* (double) MIN_NUM_POINTS_PER_ONTIME/duty_cycle_percent));
   	}

if ((duty_cycle_percent + ttran_rise_percent/2.0 + ttran_fall_percent/2.0) > 100.0)
   	{
   	printf("Sum of rise time (%.1f%%), fall time (%.1f%%), and duty cycle (%.1f%%) exceeds 100%%.\n",
   	ttran_rise_percent,ttran_fall_percent,duty_cycle_percent);
   	return_status = EXIT_FAILURE;
   	}

if ((duty_cycle_percent - ttran_fall_percent/2.0) < 0.0)
   	{
   	printf("To create duty cycle of %.1f%%, fall time must be less than %.1f%%, read fall time of %.1f%%.\n",
   	duty_cycle_percent,2.0*duty_cycle_percent,ttran_fall_percent);
   	return_status = EXIT_FAILURE;
   	}

if ((duty_cycle_percent - ttran_rise_percent/2.0) < 0.0)
   	{
   	printf("To create duty cycle of %.1f%%, rise time must be less than %.1f%%, read rise time of %.1f%%.\n",
   	duty_cycle_percent,2.0*duty_cycle_percent,ttran_rise_percent);
   	return_status = EXIT_FAILURE;
   	}

if ((duty_cycle_percent - ttran_rise_percent/2.0 - ttran_fall_percent/2.0 ) < 0.0)
   	{
   	printf("To create duty cycle of %.1f%%, sum of rise time and fall time must be less than %.1f%%, read rise time of %.1f%% and fall time of %.1f%%.\n",
   	duty_cycle_percent,2.0*duty_cycle_percent,ttran_rise_percent,ttran_fall_percent);
   	return_status = EXIT_FAILURE;
   	}

if ((vout_bandwidth_multiplier < 0.0) || (vout_bandwidth_multiplier > MAXIMUM_BANDWIDTH_MULTIPLIER))
	{
	printf("Entered value of %.2f for output square wave RC filter bandwidth must be",vout_bandwidth_multiplier);
	printf(" greater or equal to 0 and less than or equal to %.0f).\n",MAXIMUM_BANDWIDTH_MULTIPLIER);
	return_status = EXIT_FAILURE;
	}

if (vout_bandwidth_multiplier != 0.0)
	{
	if (num_points_per_period < 10.0*vout_bandwidth_multiplier)
   	{
		printf("Enter at least %.0f number of points per period to allow for\n",
		10.0*vout_bandwidth_multiplier);
		printf("output square wave RC filter bandwidth of %s, read value of %ld.\n",
		add_units_2(vout_bandwidth_multiplier*freq_Hz,2,0,0,"Hz",value_string[0]),num_points_per_period);
		return_status = EXIT_FAILURE;
		}
	}
else
	{
	if (num_points_per_period < MIN_NUM_POINTS_PER_PERIOD)
   	{
		printf("Enter at least %d number of points per period, read value of %ld.\n",MIN_NUM_POINTS_PER_PERIOD,num_points_per_period);
		return_status = EXIT_FAILURE;
		}
	}

if ((num_periods_to_plot > num_periods) || (num_periods_to_plot < 1))
	{
	printf("Number of periods to plot must be greater than 1 and less than or equal to num_periods (%ld).\n",num_periods);
	return_status = EXIT_FAILURE;
	}
	
if (yes_no(pyes_no_psd_string,pperform_psd_analysis_flag) != EXIT_SUCCESS)
	{
	printf("Please enter \"yes\" or \"no\" to perform PSD analysis\n");
	return_status = EXIT_FAILURE;
	}

if (yes_no(pyes_no_tie_string,pperform_tie_analysis_flag) != EXIT_SUCCESS)
	{
	printf("Please enter \"yes\" or \"no\" to perform TIE analysis\n");
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
	if (1.0/(num_points_per_period*freq_Hz) > 1.0/(4.0*noise_bandwidth_Hz))
   	{
		printf("Enter more than %.0f number of points per period for noise bandwidth of %1.2e, read value of %ld.\n",4.0*noise_bandwidth_Hz/freq_Hz,noise_bandwidth_Hz,num_points_per_period);
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