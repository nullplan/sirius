#include <pthread.h>
#include <errno.h>

int pthread_mutexattr_settype(pthread_mutexattr_t *ma, int v)
{
    if (v >= 3u)
        return EINVAL;
    ma->__flg = (ma->__flg & ~3) | v;
    return 0;
}
