#include <pthread.h>
int pthread_barrierattr_init(pthread_barrierattr_t *ba)
{
    *ba = (pthread_barrierattr_t){0};
    return 0;
}
