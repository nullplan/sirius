#include <stdio.h>

FILE *fdopen(int fd, const char *mode)
{
    int flags = __fopen_flags(mode);
    if (flags == -1) return 0;

    return __fdopen_flg(fd, flags);
}
