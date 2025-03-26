#include <unistd.h>
#include <fcntl.h>
#include "syscall.h"

int rmdir(const char *fn)
{
    #ifdef SYS_rmdir
    return syscall(SYS_rmdir, fn);
    #else
    return syscall(SYS_unlinkat, AT_FDCWD, fn, AT_REMOVEDIR);
    #endif
}
