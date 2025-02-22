#include <sys/uio.h>
#include "syscall.h"

ssize_t writev(int fd, const struct iovec *iov, int iovlen)
{
    return syscall(SYS_writev, fd, iov, iovlen);
}
