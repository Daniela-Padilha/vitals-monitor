/* move cursor to position row, col */
printf("\033[%d;%dH", row, col);

/* clear entire screen */
printf("\033[2J");

/* clear from cursor to end of line */
printf("\033[K");

/* colours */
printf("\033[31m");   /* red */
printf("\033[32m");   /* green */
printf("\033[33m");   /* yellow */
printf("\033[0m");    /* reset to default */

/* bold */
printf("\033[1m");
```

/* at the start of display_update */
printf("\033[H");   /* move cursor to top-left */

The dashboard needs three functions:

`display_init(void)` — clears the screen, hides the cursor (`\033[?25l`), sets up the initial layout. Returns nothing.

`display_update(t_reading_pair *max_pair, t_reading_pair *sht_pair, t_anomaly_state *state, double bpm)` — moves cursor to top of dashboard area and redraws all values in place. Takes both sensor pairs plus the computed BPM from `peak_detect`.

`display_destroy(void)` — shows the cursor again (`\033[?25h`), moves to bottom of dashboard, prints a clean exit message.

The layout should look roughly like this when rendered:
```
╔══════════════════════════════╗
║      ft_vitals monitor       ║
╠══════════════════════════════╣
║  Heart Rate:    72 BPM       ║
║  SpO2:          98 %         ║
║  Temperature:   36.5 °C      ║
║  Humidity:      55 %         ║
╠══════════════════════════════╣
║  Status:        OK           ║
╚══════════════════════════════╝