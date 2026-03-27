#include "../inc/sim_backend.h"
#include <time.h>

int	hal_init(t_sensor *sensor, t_sensor_id id)
{
	struct timespec	ts;
	int64_t			ms;

	if (!sensor)
		return (-1);
	clock_gettime(CLOCK_MONOTONIC, &ts);
	ms = (int64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;

	sensor->sensor = id;
	sensor->start_ms = ms;
	sensor->connected = SENSOR_CONNECTED;
	sensor->last_read_ms = ms;
	sensor->heart_rate = 72.0;
	sensor->scenario = SCENARIO_NORMAL;
	sensor->noise_seed = 42;
	return (0);
}

int		hal_read(t_sensor *sensor, t_reading_pair *pair)
{
	double			elapsed_ms;
	double			current_ms;
	double			cycle_ms;
	double			t;
	struct timespec	ts;

	if (!sensor || !pair)
		return (-1);
	if (sensor->connected == SENSOR_DISCONNECTED)
		return (-1);
	
	/* Compute the position in the current cycle */
	clock_gettime(CLOCK_MONOTONIC, &ts);
	current_ms = (double)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
	cycle_ms = 60000.0 / sensor->heart_rate;
	elapsed_ms = current_ms - sensor->start_ms;
	t = fmod(elapsed_ms, cycle_ms) / cycle_ms;

	/* Model generation */
	if (sensor->sensor == SENSOR_MAX30102)
	{
		/* PPG */
		pair->first.sensor = sensor->sensor;
		pair->first.value = ppg_model(t, sensor);
		snprintf(pair->first.unit, sizeof(pair->first.unit), "BPM");
		pair->first.timestamp_ms = (int64_t)current_ms;
		/* SPO2 */
		pair->second.sensor = sensor->sensor;
		pair->second.value = spo2_model(elapsed_ms, sensor);
		snprintf(pair->second.unit, sizeof(pair->second.unit), "Sp02");
		pair->second.timestamp_ms = (int64_t)current_ms;
		sensor->last_read_ms = (int64_t)current_ms;
	}
	else if (sensor->sensor == SENSOR_SHT31)
	{
		/* TEMP */
		pair->first.sensor = sensor->sensor;
		pair->first.value = temp_model(elapsed_ms, sensor);
		snprintf(pair->first.unit, sizeof(pair->first.unit), "degC");
		pair->first.timestamp_ms = (int64_t)current_ms;
		/* HUMI */
		pair->second.sensor = sensor->sensor;
		pair->second.value = humidity_model(elapsed_ms, sensor);
		snprintf(pair->second.unit, sizeof(pair->second.unit), "RH");
		pair->second.timestamp_ms = (int64_t)current_ms;
		sensor->last_read_ms = (int64_t)current_ms;
	}
	return (0);
}

void	hal_destroy(t_sensor *sensor)
{
	if (!sensor)
		return ;
	sensor->connected = SENSOR_DISCONNECTED;
	sensor->heart_rate = 0.0;
}

int		hal_is_connected(t_sensor *sensor)
{
	if (!sensor)
		return(SENSOR_DISCONNECTED);
	return (sensor->connected);
}

double	gaussian_noise(double stddev, t_sensor *sensor)
{
	double	u1;
	double	u2;

	if (!sensor)
		return (-1);
	u1 = ((double)rand_r(&sensor->noise_seed) + 1.0) / ((double)RAND_MAX + 1.0);
	u2 = ((double)rand_r(&sensor->noise_seed) + 1.0) / ((double)RAND_MAX + 1.0);
	return (stddev * sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2));
}

double	ppg_model(double t, t_sensor *sensor)
{
	double	systolic;
	double	decay;
	double	dicrotic;
	double	value;

	if (!sensor)
		return (-1);
	/* systolic rise */
	systolic = exp(-PPG_PEAK_SHARPNESS * (t - PPG_PEAK_POSITION) * (t - PPG_PEAK_POSITION));
		
	/* diastolic  */
	decay = exp(-PPG_DECAY_RATE  * t);

	/* dicrotic notch */
	dicrotic = PPG_DICROTIC_AMP * sin(2.0 * M_PI * PPG_DICROTIC_FREQ * (t - PPG_DICROTIC_ONSET))
				* exp(-PPG_DICROTIC_DECAY * (t - PPG_DICROTIC_ONSET));
	/* notch only happens after the systolic on the diastolic */
	if (t < PPG_DICROTIC_ONSET)
    	dicrotic = 0.0;
	/* final value */
	value = (systolic + dicrotic) * decay + gaussian_noise(PPG_NOISE_STDDEV, sensor);
	return (value);
}

double temp_model(double elapsed_ms, t_sensor *sensor)
{
	double	temp;
	double	elapsed_s;
	double	frequency;

	if (!sensor)
		return (-1);
	elapsed_s = elapsed_ms / 1000.0;
	/* temp fluctuation each 5 min */
	frequency = 1.0 / FIVE_MIN_IN_S;
	temp = TEMP_BASELINE_VALUE + TEMP_DRIFT_AMP * sin(2.0 * M_PI * frequency * elapsed_s) + gaussian_noise(TEMP_NOISE_STDDEV, sensor);
	return (temp);
}

double	spo2_model(double elapsed_ms, t_sensor *sensor)
{
	double	value;
	double	elapsed_s;
	double	frequency;

	if (!sensor)
		return (-1);
	elapsed_s = elapsed_ms / 1000.0;
	/* spo2 fluctuation each 8 min */
	frequency = 1.0 / EIGHT_MIN_IN_S;
	value = SPO2_BASELINE_VALUE + SPO2_DRIFT_AMP * sin(2.0 * M_PI * frequency * elapsed_s) + gaussian_noise(SPO2_NOISE_STDDEV, sensor);
	return (value);
}

double	humidity_model(double elapsed_ms, t_sensor *sensor)
{
	double	value;
	double	elapsed_s;
	double	frequency;

	if (!sensor)
		return (-1);
	elapsed_s = elapsed_ms / 1000.0;
	/* humidity fluctuation each 10 min */
	frequency = 1.0 / TEN_MIN_IN_S;
	value = HUMI_BASELINE_VALUE + HUMI_DRIFT_AMP * sin(2.0 * M_PI * frequency * elapsed_s) + gaussian_noise(HUMI_NOISE_STDDEV, sensor);
	return (value);
}