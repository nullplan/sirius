#include <stdio.h>
#include <errno.h>

static off_t __ftello(FILE *f)
{
    __FLOCK(f);
    off_t rv = f->seek(f, 0, SEEK_CUR);
    if (rv != -1) {
        if (IS_READ(f)) rv -= f->end - f->pos;
        if (IS_WRITE(f)) rv += f->pos - f->buf;
    }
    __FUNLOCK(f);
    return rv;
}
weak_alias(ftello, __ftello);

long ftell(FILE *f) {
    off_t rv = __ftello(f);
    if ((long)rv != rv) {
        errno = EOVERFLOW;
        return -1;
    }
    return rv;
}

int fgetpos(FILE *restrict f, fpos_t *restrict pos)
{
    off_t rv = __ftello(f);
    if (rv != -1) {
        pos->__pos = rv;
        rv = 0;
    }
    return rv;
}
