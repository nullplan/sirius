#include <pthread.h>
#include <errno.h>

int pthread_rwlockattr_setpshared(pthread_rwlockattr_t *rwa, int ps)
{
    if (ps & ~PTHREAD_PROCESS_SHARED)
        return EINVAL;
    rwa->__ps = ps;
    return 0;
}
