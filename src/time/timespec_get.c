#include <time.h>

int timespec_get(struct timespec *ts, int clk)
{
    /* It just so happens that all valid clocks are exactly 1 more than their CLOCK_ counterpart. */
    if (clk != TIME_UTC && clk != TIME_MONOTONIC && clk != TIME_ACTIVE && clk != TIME_THREAD_ACTIVE)
        return 0;
    int rv = __clock_gettime(clk - 1, ts);
    return rv? 0 : clk;
}
