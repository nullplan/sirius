#include <pthread.h>

int pthread_barrierattr_getpshared(const pthread_barrierattr_t *restrict ba, int *restrict ps)
{
    *ps = ba->__ps;
    return 0;
}
