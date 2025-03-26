#include <unistd.h>
#include "syscall.h"

int ftruncate(int fd, off_t len)
{
    return syscall(SYS_ftruncate, fd, __SYSCALL_LL_O(len));
}
