#include <pthread.h>

int pthread_attr_setschedpolicy(pthread_attr_t *attr, int pol)
{
    attr->__pol = pol;
    return 0;
}
