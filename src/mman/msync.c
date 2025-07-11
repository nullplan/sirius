#include <sys/mman.h>
#include "syscall.h"

int msync(void *addr, size_t len, int mode)
{
    return syscall(SYS_msync, addr, len, mode);
}
