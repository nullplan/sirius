#include <stdio.h>

hidden int __towrite(FILE *f)
{
    if (IS_WRITE(f)) return 0;
    if (f->flags & F_NOWR) {
        f->flags |= F_ERR;
        return EOF;
    }
    f->dir = -1;
    f->pos = f->buf;
    f->end = f->buf + f->buf_size;
    return 0;
}

hidden void (*__towrite_needs_stdio_exit)(void) = __stdio_exit_needed;
