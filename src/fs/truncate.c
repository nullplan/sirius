#include <unistd.h>
#include "syscall.h"

int truncate(const char *fn, off_t len)
{
    return syscall(SYS_truncate, fn, __SYSCALL_LL_O(len));
}
