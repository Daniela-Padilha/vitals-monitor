#ifndef LOGGER_H
# define LOGGER_H

# include "anomaly.h"
# include "hal.h"
# include <stdio.h>
# include <string.h>
# include <time.h>
# include <fcntl.h>
# include <stdlib.h>

typedef struct s_logger
{
	FILE *file;
	char *filepath;

}	t_logger;

int		logger_init(t_logger *logger, const char *filepath);
int		logger_write(t_logger *logger, t_reading *reading, t_anomaly_type anomaly);
void	logger_destroy(t_logger *logger);

#endif