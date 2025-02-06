#include <pthread.h>
#include <errno.h>

int pthread_attr_setdetachstate(pthread_attr_t *attr, int ds)
{
    if (ds & ~PTHREAD_CREATE_DETACHED)
        return EINVAL;
    attr->__flags = (attr->__flags & ~PTHREAD_CREATE_DETACHED) | ds;
    return 0;
}
