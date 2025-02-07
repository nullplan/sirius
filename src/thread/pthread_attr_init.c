#include <pthread.h>
#include "libc.h"

int pthread_attr_init(pthread_attr_t *attr)
{
    *attr = (pthread_attr_t){.__ss = __default_stacksize, .__gs = DEFAULT_GUARD_SIZE };
    return 0;
}
