#include "../inc/main.h"

static void    handle_signal(int sig)
{
    if (sig == SIGUSR1)
        g_connected = 0;
    else if (sig == SIGUSR2)
        g_connected = 1;
    else if (sig == SIGINT)
        g_running = 0;
}

static int init_all(t_app *app)
{
 	if (hal_init(&app->max_sensor, SENSOR_MAX30102) == -1)
        return (-1);
    if (hal_init(&app->sht_sensor, SENSOR_SHT31) == -1)
        return (-1);
    if (filter_init(&app->filter, 5) == -1)
        return (-1);
    if (detector_init(&app->detector, 0.2) == -1)
        return (-1);
    if (logger_init(&app->logger, "log.csv") == -1)
        return (-1);
    display_init();
    signal(SIGUSR1, handle_signal);
    signal(SIGUSR2, handle_signal);
    signal(SIGINT, handle_signal);
    return (0);
}

static void run_loop(t_app *app)
{
	struct timespec	ts;
	int64_t			before_ms;
	int64_t			after_ms;
	int64_t			elapsed_ms;
	int64_t			remaining;
	double			smoothed;
	double			bpm;

	while (g_running)
	{
		clock_gettime(CLOCK_MONOTONIC, &ts);
		before_ms = (int64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
		app->max_sensor.connected = g_connected;
		hal_read(&app->max_sensor, &app->max_pair);
		hal_read(&app->sht_sensor, &app->sht_pair);
		smoothed = filter_update(&app->filter, app->max_pair.first.value);
		bpm = peak_detect(&app->detector, smoothed, before_ms);
		anomaly_check(&app->max_pair, &app->anomaly);
		anomaly_check(&app->sht_pair, &app->anomaly);
		logger_write(&app->logger, &app->max_pair.first, app->anomaly.curr);
		logger_write(&app->logger, &app->max_pair.second, app->anomaly.curr);
		logger_write(&app->logger, &app->sht_pair.first, app->anomaly.curr);
		logger_write(&app->logger, &app->sht_pair.second, app->anomaly.curr);
		app->display.max_pair = app->max_pair;
		app->display.sht_pair = app->sht_pair;
		app->display.bpm = bpm;
		app->display.state = app->anomaly;
		display_update(&app->display);

		clock_gettime(CLOCK_MONOTONIC, &ts);
		after_ms = (int64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
		elapsed_ms = after_ms - before_ms;
		remaining = INTERVAL_MS - elapsed_ms;
		if (remaining > 0)
    		usleep((useconds_t)(remaining * 1000));
	}
}

static void cleanup(t_app *app)
{
	logger_destroy(&app->logger);
	filter_destroy(&app->filter);
	display_destroy();
}

int main(void)
{
	t_app	app;

	if (init_all(&app) == -1)
		return (-1);
	run_loop(&app);
	cleanup(&app);
	return (0);
}
