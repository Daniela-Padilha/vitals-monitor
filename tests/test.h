#ifndef TEST_H
# define TEST_H

# include "../inc/anomaly.h"
# include "../inc/filter.h"
# include "../inc/logger.h"
# include "../inc/sim_backend.h"
# include <unistd.h>

# define BGRN	"\033[32;1m"
# define BRED	"\033[31;1m"
# define BCYA	"\033[36;1m"
# define BMAG	"\033[35;1m"
# define RES	"\033[0m"

# define EXPECT_EQ(actual, expected, test_name)                                  \
    do {                                                                        \
        int a = (actual);                                                       \
        int e = (expected);                                                     \
        if (a != e)                                                             \
        {                                                                       \
            printf(BRED "%s FAILED: expected %d, got %d\n" RES, test_name, e, a); \
            exit(EXIT_FAILURE);                                                 \
        }                                                                       \
        printf(BGRN "%s PASSED\n" RES, test_name);                              \
    } while (0)

# define EXPECT_RANGE(value, min, max, test_name)                                \
    do {                                                                        \
        double v = (value);                                                     \
        double lo = (min);                                                      \
        double hi = (max);                                                      \
        if (v <= lo || v >= hi)                                                 \
        {                                                                       \
            printf(BRED "%s FAILED at line %d: expected %f < value < %f, got %f\n" RES, \
                   test_name, __LINE__, lo, hi, v);                             \
            exit(EXIT_FAILURE);                                                 \
        }                                                                       \
        printf(BGRN "%s PASSED\n" RES, test_name);                              \
    } while (0)

int test_all_detect(void);
int test_all_filter(void);
int test_all_anomaly(void);
int	test_all_logger(void);
int	test_all_hal(void);

#endif