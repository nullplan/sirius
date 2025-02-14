#include <pthread.h>
#include <errno.h>

int pthread_setcanceltype(int ct, int *oct)
{
    if (ct & ~PTHREAD_CANCEL_ASYNCHRONOUS) return EINVAL;
    pthread_t self = __pthread_self();
    if (oct) *oct = self->cancelstate & PTHREAD_CANCEL_ASYNCHRONOUS;
    self->cancelstate = (self->cancelstate & ~PTHREAD_CANCEL_ASYNCHRONOUS) | ct;
    return 0;
}
