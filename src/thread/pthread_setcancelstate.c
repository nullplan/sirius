#include <pthread.h>
#include <errno.h>

int __pthread_setcancelstate(int cs, int *ocs)
{
    pthread_t self = __pthread_self();
    if (cs & ~PTHREAD_CANCEL_DISABLE)
        return EINVAL;
    if (ocs) *ocs = self->cancelstate & PTHREAD_CANCEL_DISABLE;
    self->cancelstate = (self->cancelstate & ~PTHREAD_CANCEL_DISABLE) | cs;
    return 0;
}
weak_alias(pthread_setcancelstate, __pthread_setcancelstate);
