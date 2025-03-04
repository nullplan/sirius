#include <pthread.h>

int pthread_rwlockattr_init(pthread_rwlockattr_t *rwa)
{
    *rwa = (pthread_rwlockattr_t){0};
    return 0;
}
