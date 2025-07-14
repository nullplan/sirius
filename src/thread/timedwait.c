#include "futex.h"
#include "syscall.h"
#include "cpu.h"
#include "libc.h"
#include <time.h>
#include <pthread.h>

static void cleanup(void *p)
{
    a_dec((volatile int *)p);
}

hidden int __timedwait_cp(volatile int *fut, volatile int *waiters, int val, int priv, const struct timespec *absto, int clockid)
{
    int rv;
    if (priv) priv = FUTEX_PRIVATE_FLAG;
    do {
        struct timespec relto = {0}, *to = 0;
        if (absto) {
            if (absto->tv_nsec >= 1000000000ull) return -EINVAL;
            struct timespec now;
            if (__clock_gettime(clockid, &now)) return -errno;
            relto.tv_sec = absto->tv_sec - now.tv_sec;
            relto.tv_nsec = absto->tv_nsec - now.tv_nsec;
            if (relto.tv_nsec < 0)
            {
                relto.tv_nsec += 1000000000;
                relto.tv_sec--;
            }
            if (relto.tv_sec < 0) return -ETIMEDOUT;
            to = &relto;
        }
        a_inc(waiters);
        pthread_cleanup_push(cleanup, (void *)waiters);
        int op = FUTEX_WAIT;
        rv = __syscall_cp(SYS_futex, fut, op | priv, val, to);
        if (rv == -ENOSYS && priv)
            rv = __syscall_cp(SYS_futex, fut, op, val, to);
        pthread_cleanup_pop(1);
    } while (rv == -ETIMEDOUT || (rv == -EINTR && !__eintr_valid));
    return rv;
}

hidden int __timedwait(volatile int *fut, volatile int *waiters, int val, int priv, const struct timespec *absto, int clockid)
{
    int cs;
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cs);
    int rv = __timedwait_cp(fut, waiters, val, priv, absto, clockid);
    pthread_setcancelstate(cs, 0);
    return rv;
}
