#include <pthread.h>

int pthread_rwlock_init(pthread_rwlock_t *restrict rw, const pthread_rwlockattr_t *restrict rwa)
{
    *rw = (pthread_rwlock_t){.__ps = rwa? rwa->__ps : 0};
    return 0;
}
