#include <sys/uio.h>
#include "syscall.h"

ssize_t readv(int fd, const struct iovec *iov, int iovlen)
{
    return syscall(SYS_readv, fd, iov, iovlen);
}
