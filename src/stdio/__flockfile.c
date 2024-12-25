#include <stdio.h>
#include <pthread.h>
#include "cpu.h"

hidden int __flockfile(FILE *f)
{
    pthread_t self = __pthread_self();
    if (f->lock == -1 || (f->lock & FUTEX_TID_MASK) == self->tid) return 0;
    for (;;) {
        int v = f->lock;
        if (!v && !a_cas(&f->lock, 0, self->tid)) return 1;
        if (!(v & FUTEX_WAITERS) && a_cas(&f->lock, v, v|FUTEX_WAITERS)) continue;
        __futex_wait(&f->lock, 1, v | FUTEX_WAITERS);
    }
}

hidden void __funlockfile(FILE *f)
{
    int v = a_swap(&f->lock, 0);
    if (v & FUTEX_WAITERS) __futex_wake(&f->lock, 1, 1);
}

