#include <pthread.h>

int pthread_mutexattr_getprotocol(const pthread_mutexattr_t *restrict ma, int *restrict v)
{
    *v = ma->__flg & PTHREAD_PRIO_INHERIT;
    return 0;
}

int pthread_mutexattr_getpshared(const pthread_mutexattr_t *restrict ma, int *restrict v)
{
    *v = ma->__flg & PTHREAD_PROCESS_SHARED;
    return 0;
}

int pthread_mutexattr_getrobust(const pthread_mutexattr_t *restrict ma, int *restrict v)
{
    *v = ma->__flg & PTHREAD_MUTEX_ROBUST;
    return 0;
}

int pthread_mutexattr_gettype(const pthread_mutexattr_t *restrict ma, int *restrict v)
{
    *v = ma->__flg & 3;
    return 0;
}
