#include <pthread.h>
#include <errno.h>
#include "libc.h"

int pthread_mutexattr_setprotocol(pthread_mutexattr_t *ma, int v)
{
    if (v & ~PTHREAD_PRIO_INHERIT)
        return EINVAL;
    if (v && !__pi_futex_works)
        return ENOTSUP;

    ma->__flg = (ma->__flg & ~PTHREAD_PRIO_INHERIT) | v;
    return 0;
}
