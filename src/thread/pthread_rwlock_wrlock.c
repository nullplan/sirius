#include <pthread.h>

hidden int __pthread_rwlock_wrlock(pthread_rwlock_t *rw)
{
    return __pthread_rwlock_timedwrlock(rw, 0);
}
weak_alias(pthread_rwlock_wrlock, __pthread_rwlock_wrlock);
