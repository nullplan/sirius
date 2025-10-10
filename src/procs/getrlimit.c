#include <sys/resource.h>
#include "syscall.h"

int getrlimit(int res, struct rlimit *lim)
{
    int rv = __syscall(SYS_prlimit64, 0, res, 0, lim);
    #ifdef SYS_getrlimit
    if (rv == -ENOSYS)
    {
        unsigned long oldlim[2];
        rv = __syscall(SYS_getrlimit, res, oldlim);
        if (!rv) {
            lim->rlim_cur = oldlim[0] == -1ul? RLIM_INFINITY : oldlim[0];
            lim->rlim_max = oldlim[1] == -1ul? RLIM_INFINITY : oldlim[1];
        }
    }
    #endif
    return __syscall_ret(rv);
}
