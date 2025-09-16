#include <pthread.h>
#include "futex.h"
#include "syscall.h"
#include "libc.h"
hidden int __futex_wait_cp(volatile int *f, int priv, int val)
{
    int op = FUTEX_WAIT;
    if (priv) priv = FUTEX_PRIVATE_FLAG;
    int rv;
    do {
        rv = __syscall_cp(SYS_futex, f, op | priv, val, 0);
        if (rv == -ENOSYS && priv)
            rv = __syscall_cp(SYS_futex, f, op, val, 0);
    } while (rv == -EINTR && !__eintr_valid);
    return rv;
}
