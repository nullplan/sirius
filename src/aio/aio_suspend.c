#include <aio.h>
#include <time.h>
#include <errno.h>
#include "futex.h"
#include "cpu.h"

int aio_suspend(const struct aiocb *const cbs[], int ncbs, const struct timespec *to)
{
    struct timespec absto;
    int dummy_futex = 0;
    if (ncbs < 0) {
        errno = EINVAL;
        return -1;
    }
    if (to) {
        if (to->tv_nsec >= 1000000000ul)
        {
            errno = EINVAL;
            return -1;
        }
        clock_gettime(CLOCK_MONOTONIC, &absto);
        absto.tv_sec += to->tv_sec;
        absto.tv_nsec += to->tv_nsec;
        if (absto.tv_nsec >= 1000000000) {
            absto.tv_nsec -= 1000000000;
            absto.tv_sec++;
        }
        to = &absto;
    }

    int nonzerocb = 0;
    struct aiocb *cb;
    for (int i = 0; i < ncbs; i++)
        if (cbs[i]) {
            cb = (void *)cbs[i];
            nonzerocb++;
        }

    for (;;) {
        for (int i = 0; i < ncbs; i++)
            if (cbs[i] && aio_error((void *)cbs[i]) != EINPROGRESS)
                return 0;
        volatile int *fut;
        int val;

        if (nonzerocb == 0) {
            fut = &dummy_futex;
            val = 0;
        } else if (nonzerocb == 1) {
            fut = &cb->__err;
            val = a_cas(fut, EINPROGRESS, EINPROGRESS | 0x80000000u);
            if ((val & 0x7fffffff) != EINPROGRESS)
                continue;
            val |= 0x80000000u;
        } else {
            int tid = __pthread_self()->tid;
            extern hidden int __aio_notify;
            fut = &__aio_notify;
            val = a_swap(&__aio_notify, tid);
            if (!val) val = tid;
        }
        int rv = __timedwait_cp(fut, &(int){0}, val, 1, to, CLOCK_MONOTONIC);
        if (rv == -ETIMEDOUT || rv == -EINTR) {
            if (rv == -ETIMEDOUT)
                rv = -EAGAIN;
            errno = -rv;
            return -1;
        }
    }
}
