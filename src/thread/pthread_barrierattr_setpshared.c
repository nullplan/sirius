#include <pthread.h>
#include <errno.h>

int pthread_barrierattr_setpshared(pthread_barrierattr_t *ba, int ps)
{
    if (ps & ~PTHREAD_PROCESS_SHARED)
        return EINVAL;
    ba->__ps = ps;
    return 0;
}
