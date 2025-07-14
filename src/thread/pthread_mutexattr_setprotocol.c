#include <pthread.h>
#include <errno.h>
#include "syscall.h"
#include "futex.h"
#include "cpu.h"

int pthread_mutexattr_setprotocol(pthread_mutexattr_t *ma, int v)
{
    if (v & ~PTHREAD_PRIO_INHERIT)
        return EINVAL;
    if (v) {
        static int pi_futex_works;
        if (!pi_futex_works) {
            int val = 1;
            if (__syscall(SYS_futex, &(int){0}, FUTEX_UNLOCK_PI) == -ENOSYS)
                val = -1;
            a_swap(&pi_futex_works, val);
        }
        if (pi_futex_works < 0)
            return ENOTSUP;
    }
    ma->__flg = (ma->__flg & ~PTHREAD_PRIO_INHERIT) | v;
    return 0;
}
