#include "test.h"

static void	test_null_sensor(void)
{
	EXPECT_EQ(hal_init(NULL, SENSOR_MAX30102), -1, "hal_init_null_returns_-1");
}

static void	test_disconnected_sensor(void)
{
	t_sensor		sensor;
	t_reading_pair	pair;

	hal_init(&sensor, SENSOR_MAX30102);
	sensor.connected = SENSOR_DISCONNECTED;
	EXPECT_EQ(hal_read(&sensor, &pair), -1, "disconnected_sensor_read_returns_-1");
}

static void	test_ppg_range(void)
{
	t_sensor		sensor;
	t_reading_pair	pair;
	int				i;

	hal_init(&sensor, SENSOR_MAX30102);
	i = 0;
	while (i < 100)
	{
		hal_read(&sensor, &pair);
		EXPECT_RANGE(pair.first.value, -0.1, 1.1, "ppg_value_in_range");
		EXPECT_RANGE(pair.second.value, 85.0, 100.0, "spo2_value_in_range");
		usleep(10000);
		i++;
	}
}

static void	test_temp_range(void)
{
	t_sensor		sensor;
	t_reading_pair	pair;
	int				i;

	hal_init(&sensor, SENSOR_SHT31);
	i = 0;
	while (i < 50)
	{
		hal_read(&sensor, &pair);
		EXPECT_RANGE(pair.first.value, 34.0, 39.0, "temp_value_in_range");
		EXPECT_RANGE(pair.second.value, 40.0, 70.0, "humidity_value_in_range");
		usleep(10000);
		i++;
	}
}

int	test_all_hal(void)
{
	printf(BCYA"\n------Starting hal tests------\n"RES);
	test_null_sensor();
	test_disconnected_sensor();
	test_ppg_range();
	test_temp_range();
    printf(BCYA"------All hal tests passed ✅ ------\n"RES);
	return (0);
}