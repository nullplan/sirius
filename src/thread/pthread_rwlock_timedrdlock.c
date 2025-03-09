#include <pthread.h>
#include <time.h>

int pthread_rwlock_timedrdlock(pthread_rwlock_t *restrict rw, const struct timespec *restrict to)
{
    return pthread_rwlock_clockrdlock(rw, CLOCK_REALTIME, to);
}
