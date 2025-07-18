#include <pthread.h>
#include <time.h>

hidden int __pthread_rwlock_timedwrlock(pthread_rwlock_t *restrict rw, const struct timespec *restrict to)
{
    return __pthread_rwlock_clockwrlock(rw, CLOCK_REALTIME, to);
}
weak_alias(pthread_rwlock_timedwrlock, __pthread_rwlock_timedwrlock);
