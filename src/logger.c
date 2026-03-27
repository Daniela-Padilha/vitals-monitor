#include "../inc/logger.h"

int		logger_init(t_logger *logger, const char *filepath)
{
	if (!logger)
		return (-1);
	logger->filepath = strdup(filepath);
	logger->file = fopen(filepath, "w");
	if (!logger->file)
	{
		free(logger->filepath);
		return (-1);
	}
	fprintf(logger->file, "timestamp_ms,sensor,value,unit,anomaly\n");
	return (0);
}

int		logger_write(t_logger *logger, t_reading *reading, t_anomaly_type anomaly)
{
	struct timespec ts;				// current time: seconds + nanoseconds
	struct tm		*tm_info;	 	// broken-down UTC time
	time_t			sec;			 // whole seconds since epoch
	char			time_buf[32];	// formatted date/time string
	const char 		*sensor_name;

	if (!logger || !reading || !logger->file)
		return (-1);
	/* get current real time */
	clock_gettime(CLOCK_REALTIME, &ts);
	/* convert seconds to UTC calendar time */
	sec = ts.tv_sec;
	tm_info = gmtime(&sec);
	/* format tm_info into time_buf */
	strftime(time_buf, sizeof(time_buf), "%Y-%m-%dT%H:%M:%S", tm_info);
	/* write timestamp + reading data into log file */
	sensor_name = (reading->sensor == SENSOR_MAX30102) ? "MAX30102" : "SHT31";
	fprintf(logger->file, "%s.%03ldZ,%d,%f,%s,%d\n",
		time_buf,
		ts.tv_nsec / 1000000,
		sensor_name,
		reading->value,
		reading->unit,
		anomaly);
	return (0);
}

void	logger_destroy(t_logger *logger)
{
	if (!logger)
		return ;
	if (logger->file)
	{
		fclose(logger->file);
		logger->file = NULL;
	}
	free(logger->filepath);
	logger->filepath = NULL;
}
