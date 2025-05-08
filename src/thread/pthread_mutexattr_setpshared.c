#include <pthread.h>
#include <errno.h>

int pthread_mutexattr_setpshared(pthread_mutexattr_t *ma, int v)
{
    if (v & ~PTHREAD_PROCESS_SHARED)
        return EINVAL;
    ma->__flg = (ma->__flg & ~PTHREAD_PROCESS_SHARED) | v;
    return 0;
}
