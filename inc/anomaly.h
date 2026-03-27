#ifndef ANOMALY_H
# define ANOMALY_H

# include "hal.h"
# include <string.h>

# define BPM_MIN_THRESHOLD	45
# define BPM_MAX_THRESHOLD	150
# define SPO2_MIN_THRESHOLD	90
# define TEMP_MAX_THRESHOLD	38.5
# define TEMP_MIN_THRESHOLD	35.5

typedef enum e_anomaly_type {
	ANOMALY_NONE,
	ANOMALY_BRADYCARDIA,
	ANOMALY_TACHYCARDIA,
	ANOMALY_HYPOXIA,
	ANOMALY_HYPERTHERMIA,
	ANOMALY_HYPOTHERMIA
} t_anomaly_type;

typedef struct s_anomaly_state {
	t_anomaly_type	curr;
} t_anomaly_state;

int		anomaly_check(t_reading_pair *pair, t_anomaly_state *state);
int		anomaly_is_active(t_anomaly_state *state);
void	anomaly_clear(t_anomaly_state *state);

#endif