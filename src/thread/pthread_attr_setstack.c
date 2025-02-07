#include <pthread.h>
#include <errno.h>

int pthread_attr_setstack(pthread_attr_t *attr, void *st, size_t ss)
{
    if (ss < PTHREAD_STACK_MIN)
        return EINVAL;
    attr->__addr = st;
    attr->__ss = ss;
    return 0;
}
