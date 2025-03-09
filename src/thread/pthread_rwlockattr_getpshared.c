#include <pthread.h>

int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *restrict rwa, int *restrict ps)
{
    *ps = rwa->__ps;
    return 0;
}
