#include <pthread.h>
#include <time.h>

hidden int __pthread_rwlock_timedrdlock(pthread_rwlock_t *restrict rw, const struct timespec *restrict to)
{
    return __pthread_rwlock_clockrdlock(rw, CLOCK_REALTIME, to);
}
weak_alias(pthread_rwlock_timedrdlock, __pthread_rwlock_timedrdlock);
