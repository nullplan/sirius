#include <stdio.h>
#include <fcntl.h>
#include <syscall.h>

FILE *fopen(const char *restrict n, const char *restrict m)
{
    int flg = __fopen_flags(m);
    if (flg == -1) return 0;
    int fd = __sys_open(n, flg, 0666);
    if (fd == -1) return 0;
    FILE *f = __fdopen_flg(fd, flg);
    if (!f) __syscall(SYS_close, fd);
    return f;
}
