#include <unistd.h>
#include "syscall.h"

static void dummy(int x) {}
weak_alias(__aio_close, dummy);

int close(int fd)
{
    __aio_close(fd);
    int rv = __syscall_cp(SYS_close, fd);
    if (rv == -EINTR)
        rv = -EINPROGRESS;
    return __syscall_ret(rv);
}
