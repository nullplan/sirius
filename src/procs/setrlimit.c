#include <sys/resource.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include "syscall.h"

#ifdef SYS_setrlimit
struct rlimit_arg {
    unsigned long lim[2];
    int res;
    int rv;
};

static void do_setrlimit(void *a)
{
    struct rlimit_arg *arg = a;
    if (arg->rv >= 0)
    {
        int rv = __syscall(SYS_setrlimit, arg->res, arg->lim);
        if (rv && !arg->rv) abort();
        arg->rv = rv;
    }
}
#endif

int setrlimit(int res, const struct rlimit *lim)
{
    int rv = __syscall(SYS_prlimit64, 0, res, lim, 0);
    #ifdef SYS_setrlimit
    if (rv == -ENOSYS) {
        struct rlimit_arg arg;
        arg.lim[0] = lim->rlim_cur >= ULONG_MAX? ULONG_MAX : lim->rlim_cur;
        arg.lim[1] = lim->rlim_max >= ULONG_MAX? ULONG_MAX : lim->rlim_max;
        arg.res = res;
        arg.rv = 1;
        __synccall(do_setrlimit, &arg);
        rv = arg.rv;
        if (rv > 0) rv = -EAGAIN;
    }
    #endif
    return __syscall_ret(rv);
}
