#include <pthread.h>

int pthread_attr_setguardsize(pthread_attr_t *attr, size_t gs)
{
    attr->__gs = gs;
    return 0;
}
