#include <stdio.h>
#include <fcntl.h>
#include <syscall.h>
#include <errno.h>

FILE *fdopen(int fd, const char *mode)
{
    int flags = __fopen_flags(mode);
    if (flags == -1) return 0;
    if (flags & O_APPEND) {
        int fileflags = __syscall(SYS_fcntl, fd, F_GETFL);
        if (fileflags < 0) {
            errno = -fileflags;
            return 0;
        }
        if (!(fileflags & O_APPEND))
            __syscall(SYS_fcntl, fd, F_SETFL, fileflags | O_APPEND);
    }
    return __fdopen_flg(fd, flags);
}
