#include "../inc/anomaly.h"

int	anomaly_check(t_reading_pair *pair, t_anomaly_state *state)
{
	if (!pair || !state)
		return (-1);
	state->curr = ANOMALY_NONE;
	if (pair->first.sensor == SENSOR_MAX30102)
	{
		if (pair->first.value < BPM_MIN_THRESHOLD)
			state->curr = ANOMALY_BRADYCARDIA;
		else if (pair->first.value > BPM_MAX_THRESHOLD)
			state->curr = ANOMALY_TACHYCARDIA;
	}
	else if (pair->first.sensor == SENSOR_SHT31)
	{
		if (pair->first.value < TEMP_MIN_THRESHOLD)
			state->curr = ANOMALY_HYPOTHERMIA;
		else if (pair->first.value > TEMP_MAX_THRESHOLD)
			state->curr = ANOMALY_HYPERTHERMIA;
	}
	if (pair->second.sensor == SENSOR_MAX30102)
	{
		if (pair->second.value < SPO2_MIN_THRESHOLD)
			state->curr = ANOMALY_HYPOXIA;
	}
	return (state->curr);
}

int	anomaly_is_active(t_anomaly_state *state)
{
	return (state->curr != ANOMALY_NONE);
}

void	anomaly_clear(t_anomaly_state *state)
{
	state->curr = ANOMALY_NONE;
}