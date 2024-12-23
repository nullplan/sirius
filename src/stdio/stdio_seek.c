#include <stdio.h>
#include <unistd.h>
#include <errno.h>

hidden off_t __stdio_seek(FILE *f, off_t off, int s)
{
    if (s != SEEK_SET && s != SEEK_CUR && s != SEEK_END) {
        errno = EINVAL;
        return -1;
    }
    return __lseek(f->fd, off, s);
}
