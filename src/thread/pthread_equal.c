#include <pthread.h>

static int __pthread_equal(pthread_t a, pthread_t b)
{
    return a == b;
}
weak_alias(pthread_equal, __pthread_equal);
weak_alias(thrd_equal, __pthread_equal);
