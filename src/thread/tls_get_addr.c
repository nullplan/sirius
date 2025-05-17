#include <pthread.h>

void *__tls_get_addr(const size_t *x)
{
    return (void *)(__pthread_self()->dtv[x[0]] + x[1]);
}
