#ifndef FILTER_H
# define FILTER_H

# include <stdint.h>
# include <string.h>
# include <stdlib.h>

# define	SLOPE_POSITIVE	1
# define	SLOPE_NEGATIVE	0
# define	SLOPE_UNKNOWN	-1

# define	REFRACTORY_ON	1
# define	REFRACTORY_OFF	0

# define 	MS_PER_MIN		60000.0 

typedef struct s_filter
{
	double *samples_array;
	int		sample_size;
	int		current_i;
	double	sum_of_samples;
} t_filter;

typedef struct s_detector {
	int64_t	last_peak_ms;
	int		has_first_peak;
	int		slope;
	int		refractory;
	double	threshold;
	double	previous_sample;
} t_detector;

int		filter_init(t_filter *filter, int sample_size);
int		detector_init(t_detector *detector, double threshold);
void	filter_destroy(t_filter *filter);
double	filter_update(t_filter *filter, double sample);
double	peak_detect(t_detector *detector, double smoothed, int64_t timestamp_ms);

#endif