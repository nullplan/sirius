#include <unistd.h>
#include "syscall.h"

ssize_t write(int fd, const void *buf, size_t len)
{
    return syscall_cp(SYS_write, fd, buf, len);
}
