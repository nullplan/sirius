#include <pthread.h>

pthread_t pthread_self(void)
{
    return __pthread_self();
}
