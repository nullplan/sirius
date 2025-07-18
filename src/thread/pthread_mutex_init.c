#include <pthread.h>
#include <errno.h>
#include "syscall.h"
#include "cpu.h"

hidden int __pthread_mutex_init(pthread_mutex_t *restrict m, const pthread_mutexattr_t *restrict ma)
{
    static int robust_list_works; /* ternary switch */
    if (ma && (ma->__flg & (PTHREAD_PROCESS_SHARED | PTHREAD_MUTEX_ROBUST)) == (PTHREAD_PROCESS_SHARED | PTHREAD_MUTEX_ROBUST))
    {
        if (!robust_list_works)
        {
            pthread_t tp = __pthread_self();
            tp->robust.head = (void *)&tp->robust.head;
            tp->robust.off = offsetof(struct __mtx, __lock);
            tp->robust.pending = 0;
            int val = 1;
            if (__syscall(SYS_set_robust_list, &tp->robust, sizeof tp->robust))
                val = -1;
            a_swap(&robust_list_works, val);
        }
        if (robust_list_works < 0)
            return ENOTSUP;
    }
    *m = (pthread_mutex_t){0};
    if (ma)
        m->__flg = ma->__flg;
    return 0;
}
weak_alias(pthread_mutex_init, __pthread_mutex_init);
