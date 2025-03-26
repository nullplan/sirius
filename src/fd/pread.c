#include <unistd.h>
#include "syscall.h"

ssize_t pread(int fd, void *buf, size_t len, off_t off)
{
    return syscall(SYS_pread, fd, buf, len, __SYSCALL_LL_O(off));
}
