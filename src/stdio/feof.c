#include <stdio.h>

int feof(FILE *f)
{
    __FLOCK(f);
    int rv = f->flags & F_EOF;
    __FUNLOCK(f);
    return rv;
}
