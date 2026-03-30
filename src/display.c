#include "../inc/display.h"

void	display_init(void)
{
	printf(CLEAR_ALL);
	printf(TOP_LEFT);
	printf(HIDE_CURS);
	fflush(stdout);
}

void	display_destroy(void)
{
	printf(SHOW_CURS);
	printf("\n");
	fflush(stdout);
}

static char *anomaly_to_string(t_anomaly_type type)
{
	switch (type)
	{
	case ANOMALY_BRADYCARDIA:
		return ("BRADYCARDIA");
	case ANOMALY_TACHYCARDIA:
		return ("TACHYCARDIA");
	case ANOMALY_HYPOXIA:
		return ("HYPOXIA");
	case ANOMALY_HYPERTHERMIA:
		return ("HYPERTHERMIA");
	case ANOMALY_HYPOTHERMIA:
		return ("HYPOTHERMIA");
	default:
		return ("OK");
	}
}

void display_update(t_display_data *data)
{
	printf(TOP_LEFT);
	printf("╔═══════════════════════════════════╗\n");
	printf("║           " BOLD "Vitals-monitor" RES "          ║\n");
	printf("╠═══════════════════════════════════╣\n");
	printf("║  Heart Rate:  %-15.1f BPM ║\n", data->bpm);
	printf("║  SpO2:        %-16.1f %%  ║\n", data->max_pair.second.value);
	printf("║  Temperature: %-15.1f °C  ║\n", data->sht_pair.first.value);
	printf("║  Humidity:    %-16.1f %%  ║\n", data->sht_pair.second.value);
	printf("╠═══════════════════════════════════╣\n");
	if (data->state.curr == ANOMALY_NONE)
    	printf("║            Status: " GRN "%s" RES "             ║\n", "OK");
	else
    	printf("║            Status: " RED "%s" RES "             ║\n", anomaly_to_string(data->state.curr));
	printf("╚═══════════════════════════════════╝\n");
	fflush(stdout);
}