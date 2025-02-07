#include <pthread.h>
#include <errno.h>

int pthread_attr_setscope(pthread_attr_t *attr, int sc)
{
    if (sc == PTHREAD_SCOPE_PROCESS)
        return ENOTSUP;
    if (sc != PTHREAD_SCOPE_SYSTEM)
        return EINVAL;
    return 0;
}
