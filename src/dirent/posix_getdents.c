#include <dirent.h>
#include "syscall.h"

ssize_t posix_getdents(int fd, void *buf, size_t size, int flags)
{
    return syscall(SYS_getdents, fd, buf, size);
}
