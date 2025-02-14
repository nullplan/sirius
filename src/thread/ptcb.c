#include <pthread.h>

void __ptcb_push(struct __ptcb *cb)
{
    pthread_t self = __pthread_self();
    cb->__next = self->cb;
    self->cb = cb;
}

void __ptcb_pop(int execute)
{
    pthread_t self = __pthread_self();
    struct __ptcb *cb = self->cb;
    self->cb = cb->__next;
    if (execute)
        cb->__fn(cb->__a);
}
