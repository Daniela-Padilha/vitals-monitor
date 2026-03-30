#ifndef MAIN_H
# define MAIN_H

# define INTERVAL_MS 500

# include <unistd.h>
# include <signal.h>
# include <time.h>
# include <stdint.h>
# include "../inc/sim_backend.h"
# include "../inc/filter.h"
# include "../inc/anomaly.h"
# include "../inc/logger.h"
# include "../inc/display.h"

static volatile sig_atomic_t	g_connected = 1;
static volatile sig_atomic_t    g_running = 1;

typedef struct s_app
{
    t_sensor        max_sensor;
    t_sensor        sht_sensor;
    t_filter        filter;
    t_detector      detector;
    t_logger        logger;
    t_display_data  display;
    t_anomaly_state anomaly;
    t_reading_pair  max_pair;
    t_reading_pair  sht_pair;
}   t_app;

#endif