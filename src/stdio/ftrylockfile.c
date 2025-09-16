#include <stdio.h>
#include <limits.h>
#include "cpu.h"
#include "pthread.h"

hidden void __register_locked_file(FILE *f)
{
    pthread_t self = __pthread_self();
    f->lprev = 0;
    f->lnext = self->locked_files;
    if (f->lnext) f->lnext->lprev = f;
    self->locked_files = f;
}

int ftrylockfile(FILE *f)
{
    int v = f->lock;
    int tid = __pthread_self()->tid;
    if (v == -1) {
        f->lock = tid;
        __register_locked_file(f);
        return 0;
    }

    if ((v & FUTEX_NR_TID_MASK) == tid) {
        f->count++;
        return 0;
    }

    if (a_cas(&f->lock, 0, tid) == 0) {
        __register_locked_file(f);
        return 0;
    }
    return -1;
}
