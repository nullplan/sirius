#include <unistd.h>
#include "syscall.h"

int unlinkat(int dfd, const char *fn, int flag)
{
    int rv;
    if (flag & ~AT_REMOVEDIR)
        rv = -EINVAL;
    else
    #ifdef SYS_unlink
    if (dfd == -100 || *fn == '/')
        rv = __syscall(flag? SYS_rmdir : SYS_unlink, fn);
    else
    #endif
        rv = __syscall(SYS_unlinkat, dfd, fn, flag);
    return __syscall_ret(rv);
}
