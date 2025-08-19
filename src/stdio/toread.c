#include <stdio.h>

hidden int __toread(FILE *f)
{
    if (IS_READ(f)) return 0;
    if (f->flags & F_NORD) {
        f->flags |= F_ERR;
        return EOF;
    }
    f->dir = 1;
    f->pos = f->end = f->buf;
    return 0;
}

hidden void (*__toread_needs_stdio_exit)(void) = __stdio_exit_needed;
