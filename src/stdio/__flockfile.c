#include <stdio.h>
#include <pthread.h>
#include "cpu.h"

hidden int __flockfile(FILE *f)
{
    pthread_t self = __pthread_self();
    if (f->lock == -1 || (f->lock & FUTEX_TID_MASK) == self->tid) return 0;
    for (;;) {
        int v = a_cas(&f->lock, 0, self->tid);
        if (!v) return 1;
        if (!(v & FUTEX_WAITERS)) {
            if (a_cas(&f->lock, v, v|FUTEX_WAITERS) != v) continue;
            v |= FUTEX_WAITERS;
        }
        a_inc(&f->waiters);
        __futex_wait(&f->lock, 1, v);
        a_dec(&f->waiters);
    }
}

hidden void __funlockfile(FILE *f)
{
    int w = f->waiters;
    int v = a_swap(&f->lock, 0);
    if ((v & FUTEX_WAITERS) || w) __futex_wake(&f->lock, 1, 1);
}

