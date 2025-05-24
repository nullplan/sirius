#include <pthread.h>
#include <errno.h>

int pthread_mutexattr_setrobust(pthread_mutexattr_t *ma, int v)
{
    if (v & ~PTHREAD_MUTEX_ROBUST)
        return EINVAL;
    ma->__flg = (ma->__flg & ~PTHREAD_MUTEX_ROBUST) | v;
    return 0;
}
