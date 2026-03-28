#include "../inc/sim_backend.h"
#include "../inc/filter.h"
#include "../inc/anomaly.h"
#include "../inc/logger.h"
#include "../inc/display.h"
#include <signal.h>

# define INTERVAL_MS 500

static volatile sig_atomic_t g_connected = 1;

static void    handle_signal(int sig)
{
    if (sig == SIGUSR1)
        g_connected = 0;
    else if (sig == SIGUSR2)
        g_connected = 1;
}

int main(void)
{
	struct timespec	ts;
	int64_t			before_ms;
	int64_t			after_ms;
	int64_t			elapsed_ms;
	int64_t			remaining;
	t_sensor        max_sensor;
	t_sensor        sht_sensor;
	t_reading_pair  max_pair;
	t_reading_pair  sht_pair;
	t_filter		filter;
	t_detector		detector;
	t_logger		logger;
	t_display_data	display;
	t_anomaly_state	anomaly;

	if (hal_init(&max_sensor, max_sensor.sensor) == -1)
		exit(-1);
	if (hal_init(&sht_sensor, sht_sensor.sensor) == -1)
		exit(-1);
	if (filter_init(&filter, 5) == -1)
		exit(-1);
	if (detector_init(&detector, 0.2) == -1)
		exit(-1);
	if (logger_init(&logger, "../log.csv") == -1)
		exit(-1);
	display_init();

	/* signals */
	signal(SIGUSR1, handle_signal);
	signal(SIGUSR2, handle_signal);

	/* main loop */
	clock_gettime(CLOCK_MONOTONIC, &ts);
	before_ms = (int64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
	max_sensor.connected = g_connected;
	hal_read(&max_sensor, &max_pair);
	hal_read(&sht_sensor, &sht_pair);
	filter_update(&filter, 1.0);
	peak_detect(&detector, 72.0, ts);
	anomaly_check(&max_pair, &anomaly);
	logger_write(&logger, &sht_max, &anomaly);
	display_update(&display);

	clock_gettime(CLOCK_MONOTONIC, &ts);
	after_ms = (int64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
	elapsed_ms = after_ms - before_ms;
	int64_t    remaining = INTERVAL_MS - elapsed_ms;
	if (remaining > 0)
    	usleep((useconds_t)(remaining * 1000));
}