#include <pthread.h>
#include <errno.h>

int pthread_condattr_setpshared(pthread_condattr_t *ca, int v)
{
    if (v & ~PTHREAD_PROCESS_SHARED)
        return EINVAL;
    ca->__flg = v;
    return 0;
}
