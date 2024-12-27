#include <stdio.h>

static int __fseeko(FILE *f, off_t o, int w)
{
    int rv = -1;
    __FLOCK(f);
    if (IS_READ(f)) {
        if (w == SEEK_CUR) o -= f->end - f->pos;
        f->pos = f->end = f->buf;
    }
    if (IS_WRITE(f) && f->pos != f->buf) {
        f->write(f, 0, 0);
        if (f->flags & F_ERR)
            goto out;
    }
    o = f->seek(f, o, w);
    if (o != -1) {
        rv = 0;
        f->flags &= ~F_EOF;
    }
out:
    __FUNLOCK(f);
    return rv;
}
weak_alias(fseeko, __fseeko);

int fseek(FILE *f, long o, int w)
{
    return __fseeko(f, o, w);
}


int fsetpos(FILE *f, const fpos_t *pos)
{
    return __fseeko(f, pos->__pos, SEEK_SET);
}
