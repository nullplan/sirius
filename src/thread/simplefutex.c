#include <pthread.h>
#include <syscall.h>
#include "futex.h"
#include "libc.h"

hidden int __futex_wait_cp(volatile int *f, int priv, int val)
{
    int op = FUTEX_WAIT;
    if (priv && __private_futex_works) op |= FUTEX_PRIVATE_FLAG;
    int rv;
    do rv = __syscall_cp(SYS_futex, f, op, val, 0);
    while (rv == -EINTR && !__eintr_valid);
    return rv;
}

hidden int __futex_wait(volatile int *f, int priv, int val)
{
    int cs;
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cs);
    int rv = __futex_wait_cp(f, priv, val);
    pthread_setcancelstate(cs, 0);
    return rv;
}


hidden int __futex_wake(volatile int *f, int priv, int val)
{
    int op = FUTEX_WAKE;
    if (priv && __private_futex_works) op |= FUTEX_PRIVATE_FLAG;
    return __syscall(SYS_futex, f, op, val, 0);

}
