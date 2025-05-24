#include <pthread.h>
#include <time.h>
#include <errno.h>

int pthread_condattr_setclock(pthread_condattr_t *ca, clockid_t clk)
{
    if ((clk < 0 && (clk & 3) < 3)
            || clk > CLOCK_TAI
            || clk == CLOCK_PROCESS_CPUTIME_ID
            || clk == CLOCK_THREAD_CPUTIME_ID)
        return EINVAL;
    ca->__clk = clk;
    return 0;
}
