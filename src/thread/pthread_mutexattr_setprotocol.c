#include <pthread.h>
#include <errno.h>

int pthread_mutexattr_setprotocol(pthread_mutexattr_t *ma, int v)
{
    if (v & ~PTHREAD_PRIO_INHERIT)
        return EINVAL;

    ma->__flg = (ma->__flg & ~PTHREAD_PRIO_INHERIT) | v;
    return 0;
}
