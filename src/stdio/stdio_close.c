#include <stdio.h>
#include <syscall.h>

hidden int __stdio_close(FILE *f)
{
    int rv = __syscall(SYS_close, f->fd);
    if (rv == -EINTR) rv = 0;
    f->fd = -1;
    return __syscall_ret(rv);
}
