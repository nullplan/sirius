#include <pthread.h>
#include <errno.h>

int pthread_attr_setstacksize(pthread_attr_t *attr, size_t ss)
{
    if (ss < PTHREAD_STACK_MIN)
        return EINVAL;
    attr->__ss = ss;
    return 0;
}
