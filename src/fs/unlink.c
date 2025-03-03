#include <unistd.h>
#include "syscall.h"

int unlink(const char *fn)
{
#ifdef SYS_unlink
    return syscall(SYS_unlink, fn);
#else
    return syscall(SYS_unlinkat, -100, fn, 0);
#endif
}
