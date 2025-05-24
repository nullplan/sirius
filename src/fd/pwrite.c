#include <unistd.h>
#include "syscall.h"

ssize_t pwrite(int fd, const void *buf, size_t len, off_t off)
{
    return syscall_cp(SYS_pwrite, fd, buf, len, __SYSCALL_LL_O(off));
}
