#include "test.h"

static t_reading	make_reading(t_sensor_id id, double value, const char *unit)
{
	t_reading	r;

	r.sensor = id;
	r.value = value;
	snprintf(r.unit, sizeof(r.unit), "%s", unit);
	r.timestamp_ms = 1000;
	return (r);
}

static void	test_null_logger(void)
{
	EXPECT_EQ(logger_init(NULL, "out.csv"), -1, "null_logger_returns_-1");
}

static void	test_unwritable_path(void)
{
	t_logger	logger;

	EXPECT_EQ(logger_init(&logger, "/root/forbidden.csv"), -1, "unwritable_path_returns_-1");
}

static void	test_csv_format(void)
{
	t_logger	logger;
	t_reading	reading;
	FILE		*fp;
	int			fd;
	char		line[256];
	int			found;

	EXPECT_EQ(logger_init(&logger, "/tmp/ft_vitals_test.csv"), 0, "logger_init_succeeds");

	reading = make_reading(SENSOR_MAX30102, 72.5, "BPM");
	EXPECT_EQ(logger_write(&logger, &reading, ANOMALY_NONE), 0,
		"logger_write_succeeds");
	logger_destroy(&logger);
	

	/* read back and check format */
	fd = open("/tmp/ft_vitals_test.csv", O_RDONLY);
	if (fd != -1)
		EXPECT_EQ(fd, 3, "csv_file_readable");

	found = 0;
	fp = fopen("/tmp/ft_vitals_test.csv", "r");
	while (fgets(line, sizeof(line), fp))
	{
		/* check for ISO 8601 T separator and Z suffix */
		if (strchr(line, 'T') && strchr(line, 'Z'))
			found = 1;
	}
	fclose(fp);
	EXPECT_EQ(found, 1, "csv_contains_iso8601_timestamp");
}

int	test_all_logger(void)
{
	printf(BCYA"\n------Starting logger tests------\n"RES);
	test_null_logger();
	test_unwritable_path();
	test_csv_format();
	printf(BCYA "------All logger tests passed ✅ ------\n" RES);
	return (0);
}