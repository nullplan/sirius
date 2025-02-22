#include <sys/stat.h>
#include "syscall.h"

int mkdirat(int dfd, const char *name, mode_t m)
{
    int rv;
#ifdef SYS_mkdir
    if (dfd == -100 || *name == '/')
        rv = __syscall(SYS_mkdir, name, m);
    else
#endif
        rv = __syscall(SYS_mkdirat, dfd, name, m);
    return __syscall_ret(rv);
}
