#include <threads.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>

int cnd_timedwait(cnd_t *restrict cnd, mtx_t *restrict mtx, const struct timespec *restrict to)
{
    int rv = __pthread_cond_clockwait(cnd, mtx, CLOCK_REALTIME, to);
    return rv == 0? thrd_success : rv == ETIMEDOUT? thrd_timedout : thrd_error;
}
