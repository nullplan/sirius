#include <pthread.h>

int pthread_condattr_init(pthread_condattr_t *ca)
{
    *ca = (pthread_condattr_t){0};
    return 0;
}
