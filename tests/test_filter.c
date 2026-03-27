#include "test.h"

static void    test_ring_buffer_wraps(void)
{
    t_filter    filter;
    double      result;

    filter_init(&filter, 3);
    filter_update(&filter, 0.38);
    filter_update(&filter, 0.41);
    filter_update(&filter, 0.36);
    result = filter_update(&filter, 0.52);
    /* oldest (0.38) replaced by 0.52 */
    /* mean should be (0.41 + 0.36 + 0.52) / 3 = 0.43 */
	EXPECT_EQ(result, 0.43, "test_ring_buffer_wraps");
    filter_destroy(&filter);
}

static void    test_null_filter(void)
{
    EXPECT_EQ(filter_init(NULL, 10), -1, "test_null_filter");
}

static void    test_zero_sample_size(void)
{
    t_filter    filter;

    EXPECT_EQ(filter_init(&filter, 0), -1, "test_zero_sample_size");
    EXPECT_EQ(filter_init(&filter, -1), -1, "test_zero_sample_size");
}

int    test_all_filter(void)
{
    printf(BCYA"\n------Starting filter tests------\n"RES);
    test_null_filter();
    test_zero_sample_size();
    test_ring_buffer_wraps();
    printf(BCYA"------All filter tests passed ✅ ------\n"RES);
    return (0);
}
