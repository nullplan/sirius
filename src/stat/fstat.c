#define _GNU_SOURCE
#include <sys/stat.h>
#include <fcntl.h>

hidden int __fstat(int fd, struct stat *st)
{
    return __fstatat(fd, "", st, AT_EMPTY_PATH);
}
weak_alias(fstat, __fstat);
