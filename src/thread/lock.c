#include <pthread.h>
#include <limits.h>
#include "cpu.h"

/* design proudly stolen from musl.
 *
 * Lock word is an int. Values mean:
 * 0 - lock is free and uncontended
 * x - lock is free, but contended x times.
 * INT_MIN+x - lock is taken and contended x times.
 *
 * Equivalently: highest bit is lock bit, remainder is contention count.
 *
 * Turning waiters count into contention count makes lock handoff easier.
 *
 * Locking therefore means trying to transition the lock bit from 0 to 1.
 * The one who does that is the lock holder.
 *
 * All other threads add themselves to the contention count and wait for
 * the value to turn non-negative. If the value is negative, they wait
 * on the lock and opportunistically assume the lock was returned
 * by some thread (rather than being awakened spuriously or something).
 *
 * On unlock, if the word was INT_MIN + 1 before, then it is 0 after, so
 * no contender to be awoken, and so we can save the syscall.
 */

hidden void __lock(struct lock *lck)
{
    int v = a_cas(&lck->l, 0, INT_MIN + 1);
    if (!v) return;

    a_inc(&lck->l);
    v = lck->l;

    for (;;) {
        if (v < 0) {
            __futex_wait(&lck->l, 1, v);
            v -= INT_MIN + 1;
        }
        int n = a_cas(&lck->l, v, v + INT_MIN);
        if (n == v) return;
        v = n;
    }
}

hidden void __unlock(struct lock *lck)
{
    int v;

    v = lck->l;
    for (;;) {
        int n = a_cas(&lck->l, v, v - (INT_MIN + 1));
        if (n == v) break;
        v = n;
    }

    if (v != INT_MIN + 1)
        __futex_wake(&lck->l, 1, 1);
}
