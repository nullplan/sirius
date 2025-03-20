#include <unistd.h>
#include <fcntl.h>
#include "syscall.h"

ssize_t readlink(const char *restrict path, char *restrict buf, size_t len)
{
    #ifdef SYS_readlink
    return syscall(SYS_readlink, path, buf, len);
    #else
    return syscall(SYS_readlinkat, AT_FDCWD, path, buf, len);
    #endif
}
