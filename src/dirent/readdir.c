#include <dirent.h>
#include "syscall.h"

struct dirent *readdir(DIR *d)
{
    if (d->pos == d->end)
    {
        long count = syscall(SYS_getdents, d->fd, d->buf, DIRBUFSIZ);
        if (count <= 0) return 0;
        d->pos = d->buf;
        d->end = d->buf + count;
    }
    struct dirent *rv = (void *)d->pos;
    d->pos += rv->d_reclen;
    d->tell = rv->d_off;
    return rv;
}
