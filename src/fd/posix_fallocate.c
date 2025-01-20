#include <fcntl.h>
#include "syscall.h"

int posix_fallocate(int fd, off_t start, off_t len)
{
    return syscall(SYS_fallocate, fd, 0, __SYSCALL_LL_E(start), __SYSCALL_LL_E(len));
}
