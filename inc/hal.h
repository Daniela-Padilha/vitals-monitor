#ifndef HAL_H
# define HAL_H

# include <stdint.h>

typedef enum e_sensor_id {
	SENSOR_MAX30102,
	SENSOR_SHT31,
}	t_sensor_id;

typedef struct s_reading {
	t_sensor_id	sensor;
	double		value;
	char		unit[8];
	int64_t		timestamp_ms;
}	t_reading;

typedef struct s_reading_pair {
	t_reading	first;
	t_reading	second;
}	t_reading_pair;

typedef struct s_sensor	t_sensor;

int		hal_init(t_sensor *sensor, t_sensor_id id);
int		hal_read(t_sensor *sensor, t_reading_pair *pair);
void	hal_destroy(t_sensor *sensor);
int		hal_is_connected(t_sensor *sensor);

#endif