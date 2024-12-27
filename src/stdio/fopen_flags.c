#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

hidden int __fopen_flags(const char *m)
{
    int flg = 0;
    if (!strchr("rwa", *m)) {
        errno = EINVAL;
        return -1;
    }
    if (strchr(m, '+')) flg = O_RDWR;
    else if (*m == 'r') flg = O_RDONLY;
    else flg = O_WRONLY;

    if (*m == 'w') flg |= O_CREAT | O_TRUNC;
    else if (*m == 'a') flg |= O_CREAT | O_APPEND;

    if (strchr(m, 'e')) flg |= O_CLOEXEC;
    if (strchr(m, 'x')) flg |= O_EXCL;

    return flg;
}
