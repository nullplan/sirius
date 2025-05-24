#include <pthread.h>
#include <errno.h>
#include "libc.h"

int pthread_mutex_init(pthread_mutex_t *restrict m, const pthread_mutexattr_t *restrict ma)
{
    if (!__robust_list_works && ma && (ma->__flg & (PTHREAD_PROCESS_SHARED | PTHREAD_MUTEX_ROBUST)) == (PTHREAD_PROCESS_SHARED | PTHREAD_MUTEX_ROBUST))
        return ENOTSUP;
    *m = (pthread_mutex_t){0};
    if (ma)
        m->__flg = ma->__flg;
    return 0;
}
