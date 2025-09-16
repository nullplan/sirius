#include <pthread.h>
#include <syscall.h>
#include "futex.h"
#include "libc.h"

hidden int __futex_wait(volatile int *f, int priv, int val)
{
    int op = FUTEX_WAIT;
    if (priv) priv = FUTEX_PRIVATE_FLAG;
    int rv;
    do {
        rv = __syscall(SYS_futex, f, op | priv, val, 0);
        if (rv == -ENOSYS && priv)
            rv = __syscall(SYS_futex, f, op, val, 0);
    } while (rv == -EINTR && !__eintr_valid);
    return rv;
}

hidden int __futex_wake(volatile int *f, int priv, int val)
{
    int op = FUTEX_WAKE;
    if (priv) priv = FUTEX_PRIVATE_FLAG;
    int rv = __syscall(SYS_futex, f, op | priv, val, 0);
    if (rv == -ENOSYS && priv)
        rv = __syscall(SYS_futex, f, op, val, 0);
    return rv;
}
