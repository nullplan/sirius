#include <unistd.h>
#include <fcntl.h>
#include "syscall.h"

ssize_t readlinkat(int dfd, const char *restrict path, char *restrict buf, size_t len)
{
    #ifdef SYS_readlink
    if (dfd == AT_FDCWD || *path == '/')
        return syscall(SYS_readlink, path, buf, len);
    #endif
    return syscall(SYS_readlinkat, dfd, path, buf, len);
}
