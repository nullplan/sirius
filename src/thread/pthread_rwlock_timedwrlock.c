#include <pthread.h>
#include <time.h>

int pthread_rwlock_timedwrlock(pthread_rwlock_t *restrict rw, const struct timespec *restrict to)
{
    return pthread_rwlock_clockwrlock(rw, CLOCK_REALTIME, to);
}
