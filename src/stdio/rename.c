#include <stdio.h>
#include <fcntl.h>
#include <syscall.h>

int rename(const char *old, const char *new)
{
#ifdef SYS_rename
    return syscall(SYS_rename, old, new);
#else
    return syscall(SYS_renameat2, AT_FDCWD, old, AT_FDCWD, new, 0);
#endif
}
