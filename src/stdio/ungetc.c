#include <stdio.h>

int ungetc(int c, FILE *f)
{
    int rv = EOF;
    __FLOCK(f);
    if (!__toread(f) && c != EOF && f->buf - f->pos < UNGET) {
        rv = (*--(f->pos) = c);
    }
    __FUNLOCK(f);
    return rv;
}
