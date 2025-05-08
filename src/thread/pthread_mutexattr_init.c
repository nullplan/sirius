#include <pthread.h>

int pthread_mutexattr_init(pthread_mutexattr_t *ma)
{
    *ma = (pthread_mutexattr_t){0};
    return 0;
}
