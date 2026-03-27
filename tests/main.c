#include "test.h"

int main (void) {
	test_all_detect();
	test_all_filter();
	test_all_anomaly();
	test_all_logger();
	test_all_hal();
	return (0);
}
