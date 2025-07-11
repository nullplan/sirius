#include <sys/mman.h>
#include "syscall.h"

int mlock(const void *p, size_t len)
{
    return syscall(SYS_mlock, p, len);
}
