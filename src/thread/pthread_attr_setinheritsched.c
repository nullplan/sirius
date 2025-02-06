#include <pthread.h>
#include <errno.h>

int pthread_attr_setinheritsched(pthread_attr_t *attr, int is)
{
    if (is & ~PTHREAD_EXPLICIT_SCHED)
        return EINVAL;
    attr->__flags = (attr->__flags & ~PTHREAD_EXPLICIT_SCHED) | is;
    return 0;
}
