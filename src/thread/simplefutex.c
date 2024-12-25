#include <pthread.h>
#include <syscall.h>
#include "futex.h"

hidden int __futex_wait(volatile int *f, int priv, int val)
{
    int op = FUTEX_WAIT;
    if (priv) op |= FUTEX_PRIVATE_FLAG;
    int rv = __syscall(SYS_futex, f, op, val, 0);
    if (rv == -ENOSYS && priv)
        rv = __syscall(SYS_futex, f, FUTEX_WAIT, val, 0);
    return rv;
}

hidden int __futex_wake(volatile int *f, int priv, int val)
{
    int op = FUTEX_WAKE;
    if (priv) op |= FUTEX_PRIVATE_FLAG;
    int rv = __syscall(SYS_futex, f, op, val, 0);
    if (rv == -ENOSYS && priv)
        rv = __syscall(SYS_futex, f, FUTEX_WAKE, val, 0);
    return rv;

}
