#include <pthread.h>
#include <time.h>

int pthread_mutex_timedlock(pthread_mutex_t *restrict m, const struct timespec *restrict to)
{
    return pthread_mutex_clocklock(m, CLOCK_REALTIME, to);
}
