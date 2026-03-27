#include "test.h"

static t_detector make_detetor(void)
{
	t_detector d;

	detector_init(&d, 0.1);
	return (d);
}

static void test_below_threshold(void)
{
	t_detector d = make_detetor();
	double		res;

	res = peak_detect(&d, 0.05, 1000);
	EXPECT_EQ(res, -1, "below_threshold returns -1");
	EXPECT_EQ(d.slope, SLOPE_UNKNOWN, "below_threshold resets slope");
}

static void test_first_peak(void)
{
	t_detector d = make_detetor();
	double		res;

	/* simulate first peak */
	peak_detect(&d, 0.20, 100);
	peak_detect(&d, 0.30, 200);
	peak_detect(&d, 0.38, 300);
	res = peak_detect(&d, 0.30, 400);
	EXPECT_EQ(res, -1, "first_peak returns -1");
	EXPECT_EQ(d.has_first_peak, 1, "first_peak sets has_first_peak");
	EXPECT_EQ(d.last_peak_ms, 400, "first_peak stores timestamp");
}

static void test_second_peak_returns_bpm()
{
	t_detector d = make_detetor();
	double		bpm;
	
	/* simulate first peak */
	peak_detect(&d, 0.20, 0);
	peak_detect(&d, 0.38, 100);
	peak_detect(&d, 0.30, 200);

	/* wait past refractory */
	peak_detect(&d, 0.20, 600);
	peak_detect(&d, 0.38, 900);
	bpm = peak_detect(&d, 0.30, 1033);

	EXPECT_EQ(d.last_peak_ms, 1033, "second_peak stores timestamp");
	EXPECT_RANGE(bpm, 70.0, 73.0, "second_peak_bpm_in_range");
	/* first peak detected at timestamp 200ms
	second peak detected at timestamp 1033ms
	interval = 1033 - 200 = 833ms
	BPM = 60000 / 833 = 72.02 BPM */
}

static void test_refractory_blocks(void)
{
	t_detector d = make_detetor();
	double		res;

	/* simulate first peak */
	peak_detect(&d, 0.20, 0);
	peak_detect(&d, 0.38, 100);
	peak_detect(&d, 0.30, 200);

	/* attempt second peak 150ms later */
	peak_detect(&d, 0.31, 250);
	peak_detect(&d, 0.38, 300);
	res = peak_detect(&d, 0.30, 350);

	EXPECT_EQ(res, -1, "refractory_blocks_early_peak");
}

static void test_flat_signal(void)
{
	t_detector d = make_detetor();
	double		res;

	/* rising to above threshold */
	peak_detect(&d, 0.20, 100);
	peak_detect(&d, 0.35, 200);
	res = peak_detect(&d, 0.35, 300);
	EXPECT_EQ(res, -1, "flat_signal_no_peak");
	EXPECT_EQ(d.slope, SLOPE_POSITIVE, "flat_signal_slope_unchanged");
}

int	test_all_detect(void)
{
	printf(BCYA"\n------Starting detect tests------\n"RES);
	test_below_threshold();
	test_first_peak();
	test_second_peak_returns_bpm();
	test_refractory_blocks();
	test_flat_signal();
	printf(BCYA"------All peak_detect tests passed ✅ ------\n" RES);
	return (0);
}