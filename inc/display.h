#ifndef DISPLAY_H
# define DISPLAY_H

# include "sim_backend.h"
# include "anomaly.h"

# define	RED		"\033[31m"
# define	GRN		"\033[32m"
# define	YEL		"\033[33m"
# define	RES		"\033[0m"
# define	BOLD	"\033[1m"

# define	CLEAR_ALL	"\033[2J"
# define	CLEAR_LINE	"\033[K"
# define	TOP_LEFT	"\033[H"
# define	HIDE_CURS	"\033[?25l"
# define	SHOW_CURS	"\033[?25h"

typedef struct s_display_data {
	t_reading_pair	max_pair;
	t_reading_pair	sht_pair;
	double			bpm;
	t_anomaly_state	state;
} t_display_data;

void display_init(void);
void display_update(t_display_data *data);
void display_destroy(void);

// printf("\033[%d;%dH", row, col);

// ╔══════════════════════════════╗
// ║      ft_vitals monitor       ║
// ╠══════════════════════════════╣
// ║  Heart Rate:    72 BPM       ║
// ║  SpO2:          98 %         ║
// ║  Temperature:   36.5 °C      ║
// ║  Humidity:      55 %         ║
// ╠══════════════════════════════╣
// ║  Status:        OK           ║
// ╚══════════════════════════════╝

#endif