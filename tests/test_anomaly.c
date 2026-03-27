#include "test.h"

static t_reading_pair make_pair(t_sensor_id id, double first_val, double second_val)
{
	t_reading_pair pair;

	pair.first.sensor = id;
	pair.first.value = first_val;
	pair.second.sensor = id;
	pair.second.value = second_val;
	return (pair);
}

void	test_null_anomaly(void)
{
	t_anomaly_state state;
	t_reading_pair  pair;

	EXPECT_EQ(anomaly_check(NULL, &state), -1, "test_null_pair_anomaly");
	pair = make_pair(SENSOR_MAX30102, 72.0, 98.0);
	EXPECT_EQ(anomaly_check(&pair, NULL), -1, "test_null_state_anomaly");
}

static void	test_anomaly_states(void)
{
	t_anomaly_state state;
	t_reading_pair  pair;

	/* MAX30102 */
	pair = make_pair(SENSOR_MAX30102, 72.0, 98.0);
	EXPECT_EQ(anomaly_check(&pair, &state), ANOMALY_NONE, "normal_max30102_no_anomaly");

	pair = make_pair(SENSOR_MAX30102, 45.0, 98.0);
	EXPECT_EQ(anomaly_check(&pair, &state), ANOMALY_NONE, "bpm_exactly_45_not_anomaly");

	pair = make_pair(SENSOR_MAX30102, 44.9, 98.0);
	EXPECT_EQ(anomaly_check(&pair, &state), ANOMALY_BRADYCARDIA, "bpm_44.9_is_bradycardia");

	pair = make_pair(SENSOR_MAX30102, 151.0, 98.0);
	EXPECT_EQ(anomaly_check(&pair, &state), ANOMALY_TACHYCARDIA, "bpm_151_is_tachycardia");

	pair = make_pair(SENSOR_MAX30102, 72.0, 89.0);
	EXPECT_EQ(anomaly_check(&pair, &state), ANOMALY_HYPOXIA, "spo2_89_is_hypoxia");	

	/* SHT31 */
	pair = make_pair(SENSOR_SHT31, 36.5, 55.0);
	EXPECT_EQ(anomaly_check(&pair, &state), ANOMALY_NONE, "normal_sht31_no_anomaly");

	pair = make_pair(SENSOR_SHT31, 40.0, 55.0);
	EXPECT_EQ(anomaly_check(&pair, &state), ANOMALY_HYPERTHERMIA, "temp_40_is_hyperthermia");

	pair = make_pair(SENSOR_SHT31, 34.0, 55.0);
	EXPECT_EQ(anomaly_check(&pair, &state), ANOMALY_HYPOTHERMIA, "temp_34_is_hypothermia");	
}

static void	test_state_clears(void)
{
	t_anomaly_state	state;
	t_reading_pair	pair;

	/* trigger anomaly */
	pair = make_pair(SENSOR_MAX30102, 30.0, 98.0);
	anomaly_check(&pair, &state);
	EXPECT_EQ(state.curr, ANOMALY_BRADYCARDIA, "state_set_to_bradycardia");

	/* normal reading clears it */
	pair = make_pair(SENSOR_MAX30102, 72.0, 98.0);
	anomaly_check(&pair, &state);
	EXPECT_EQ(state.curr, ANOMALY_NONE, "state_cleared_after_normal");
}

int test_all_anomaly(void) 
{
	printf(BCYA"\n------Starting anomaly tests------\n"RES);
	test_null_anomaly();
	test_anomaly_states();
	test_state_clears();
	printf(BCYA"------ All anomaly tests passed ✅ ------\n"RES);
	return (0);
}
