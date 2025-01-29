#include <unistd.h>
#include "syscall.h"

static void dummy(int x) {}
weak_alias(__aio_close, dummy);

int close(int fd)
{
    __aio_close(fd);
    int rv = __syscall(SYS_close, fd);
    if (rv == -EINTR) rv = 0;
    return __syscall_ret(rv);
}

int posix_close(int fd, int flg)
{
    if (flg) {
        errno = EINVAL;
        return -1;
    }
    return close(fd);
}
