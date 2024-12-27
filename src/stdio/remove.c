#include <stdio.h>
#include <fcntl.h>
#include <syscall.h>

int remove(const char *n)
{
#ifdef SYS_unlink
    int rv = __syscall(SYS_unlink, n);
    if (rv == -EISDIR)
        rv = __syscall(SYS_rmdir, n);
#else
    int rv = __syscall(SYS_unlinkat, AT_FDCWD, n, 0);
    if (rv == -EISDIR)
        rv = __syscall(SYS_unlinkat, AT_FDCWD, n, AT_REMOVEDIR);
#endif
    return __syscall_ret(rv);

}
