#include <stdio.h>

hidden int __overflow(FILE *f, int c)
{
    if (__towrite(f)) return EOF;
    unsigned char b = c;
    if (f->pos < f->end) return *f->pos++ = b;
    if (f->write(f, &b, 1) != 1) return EOF;
    return b;
}
