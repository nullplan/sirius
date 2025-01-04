#include <stdio.h>

hidden int __underflow(FILE *f)
{
    if (__toread(f)) return EOF;
    if (f->pos != f->end) return *f->pos++;
    unsigned char b;
    if (f->read(f, &b, 1) != 1) return EOF;
    return b;
}
