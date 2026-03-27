#include "../inc/filter.h"

int	filter_init(t_filter *filter, int sample_size)
{
	if (!filter || sample_size <= 0)
		return (-1);
	filter->samples_array = calloc(sample_size, sizeof(double));
	if (!filter->samples_array)
		return (-1);
	filter->sample_size = sample_size;
	filter->current_i = 0;
	filter->sum_of_samples = 0.0;
	return (0);
}

void	filter_destroy(t_filter *filter)
{
	if (!filter)
		return
	free(filter->samples_array);
	filter->samples_array = NULL;
}

int		detector_init(t_detector *detector, double threshold)
{
	if (!detector)
		return (-1);
	detector->has_first_peak = 0;
	detector->last_peak_ms = 0;
	detector->slope = SLOPE_UNKNOWN;
	detector->refractory = 0;
	detector->threshold = threshold;
	detector->previous_sample = 0.0;
	return (0);
}

//update ring buffer and compute smoothed sample
double	filter_update(t_filter *filter, double sample)
{
	double	mean;

	filter->sum_of_samples -= filter->samples_array[filter->current_i];
	filter->samples_array[filter->current_i] = sample;
	filter->sum_of_samples += sample;
	filter->current_i += 1;
	if (filter->current_i >= filter->sample_size)
		filter->current_i = 0;
	mean = filter->sum_of_samples / filter->sample_size;
	return (mean);
}

//peak detection algorithm and compute BPM
double	peak_detect(t_detector *detector, double smoothed, int64_t timestamp_ms)
{
	int		previous_slope;
	int64_t	interval_ms;
	double	bpm;

	//threshold compliance
	if (smoothed < detector->threshold)
	{
		detector->slope = SLOPE_UNKNOWN;
		return (-1);
	}
	//refractory period
	if (detector->refractory == REFRACTORY_ON
		&& (timestamp_ms - detector->last_peak_ms) < 300)
		return (-1);
	else
		detector->refractory = REFRACTORY_OFF;
	//slope detection
	previous_slope = detector->slope;
	if (smoothed > detector->previous_sample)
		detector->slope = SLOPE_POSITIVE;
	else if (smoothed < detector->previous_sample)
		detector->slope = SLOPE_NEGATIVE;
	//peak detected, compute BPM if not the first peak
	detector->previous_sample = smoothed;
	if (previous_slope == SLOPE_POSITIVE && detector->slope == SLOPE_NEGATIVE)
	{
		/* first peak detected */
		if (detector->has_first_peak == 0)
		{
			detector->last_peak_ms = timestamp_ms;
			detector->has_first_peak = 1;
			detector->refractory = REFRACTORY_ON;
			return (-1);
		}
		/* second peak detected */
		if (detector->has_first_peak == 1)
		{
			interval_ms = timestamp_ms - detector->last_peak_ms;
			bpm = MS_PER_MIN/ interval_ms;
			detector->last_peak_ms = timestamp_ms;
			detector->refractory = REFRACTORY_ON;
			return (bpm);
		}
	}
	return (-1);
}
