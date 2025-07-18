#include <threads.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>

int mtx_timedlock(mtx_t *restrict mtx, const struct timespec *restrict ts)
{
    int rv = __pthread_mutex_clocklock(mtx, CLOCK_REALTIME, ts);
    return rv == 0? thrd_success : rv == ETIMEDOUT? thrd_timedout : thrd_error;
}
