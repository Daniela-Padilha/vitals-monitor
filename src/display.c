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

