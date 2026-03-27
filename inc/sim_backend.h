#ifndef SIM_BACKEND_H
# define SIM_BACKEND_H

# define	SENSOR_CONNECTED		1
# define	SENSOR_DISCONNECTED		0

# define	M_PI 3.14159265358979323846 /* PI value */
# define	PPG_PEAK_POSITION    0.25   /* systolic peak at 25% of cycle */
# define	PPG_PEAK_SHARPNESS   50.0   /* higher = narrower systolic peak */
# define	PPG_DECAY_RATE       4.0    /* diastolic decay speed */
# define	PPG_DICROTIC_AMP     0.25   /* dicrotic bump height relative to systolic */
# define	PPG_DICROTIC_ONSET   0.55   /* dicrotic notch position in cycle */
# define	PPG_DICROTIC_FREQ    2.5    /* dicrotic bump oscillation frequency */
# define	PPG_DICROTIC_DECAY   8.0    /* dicrotic bump damping rate */
# define	PPG_NOISE_STDDEV     0.02   /* gaussian noise standard deviation */

# define	TEMP_NOISE_STDDEV	0.01	/* gaussian noise standard deviation */
# define	TEMP_BASELINE_VALUE	36.5	/* baseline of 36.5 ºC */
# define	TEMP_DRIFT_AMP		0.3		/* how much the signal varies from the baseline */
# define	FIVE_MIN_IN_S		300.0

# define	SPO2_NOISE_STDDEV	0.05	/* gaussian noise standard deviation */
# define	SPO2_BASELINE_VALUE	98.0	/* baseline of 98% */
# define	SPO2_DRIFT_AMP		1.0		/* how much the signal varies from the baseline */
# define	EIGHT_MIN_IN_S		480.0

# define	HUMI_NOISE_STDDEV	0.2		/* gaussian noise standard deviation */
# define	HUMI_BASELINE_VALUE	55.0	/* baseline of 55% RH */
# define	HUMI_DRIFT_AMP		5.0		/* how much the signal varies from the baseline */
# define	TEN_MIN_IN_S		600.0

# include "hal.h"
# include <stdint.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>

typedef enum e_scenario {
	SCENARIO_NORMAL,
	SCENARIO_BRADYCARDIA,
	SCENARIO_TACHYCARDIA,
	SCENARIO_HYPOXIA,
} t_scenario;

typedef struct s_sensor {
	t_sensor_id		sensor;
	int64_t			start_ms;
	int				connected;
	int64_t			last_read_ms;
	double			heart_rate;		/* PPG cycle rate, unused by SHT31 */
	t_scenario		scenario;
	unsigned int	noise_seed;
}	t_sensor;

double	gaussian_noise(double stddev, t_sensor *sensor);
double	ppg_model(double t, t_sensor *sensor);
double	temp_model(double elapsed_ms, t_sensor *sensor);
double	spo2_model(double elapsed_ms, t_sensor *sensor);
double	humidity_model(double elapsed_ms, t_sensor *sensor);

#endif