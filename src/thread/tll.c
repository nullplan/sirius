#include <pthread.h>
#include "cpu.h"
#include "libc.h"
#include "futex.h"

static int waiters;

hidden void __tl_lock(void)
{
    int tid = __pthread_self()->tid;
    for (;;) {
        int v = a_cas(&__thread_list_lock, 0, tid);
        if (!v) return;
        if (!(v & FUTEX_WAITERS)) {
            if (a_cas(&__thread_list_lock, v, v | FUTEX_WAITERS) != v)
                continue;
            v |= FUTEX_WAITERS;
        }
        a_inc(&waiters);
        __futex_wait(&__thread_list_lock, 0, v);
        a_dec(&waiters);
    }
}

hidden void __tl_unlock(void)
{
    int v = a_swap(&__thread_list_lock, 0);
    if ((v & FUTEX_WAITERS) || waiters)
        __futex_wake(&__thread_list_lock, 0, 1);
}

hidden void __tl_sync(void)
{
    int v;
    for (;;) {
        v = __thread_list_lock;
        if (!v) return;
        if (!(v & FUTEX_WAITERS)) {
            if (a_cas(&__thread_list_lock, v, v | FUTEX_WAITERS) != v)
                continue;
            v |= FUTEX_WAITERS;
        }
        a_inc(&waiters);
        __futex_wait(&__thread_list_lock, 0, v);
        a_dec(&waiters);
    }
}
