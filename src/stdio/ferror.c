#include <stdio.h>

int ferror(FILE *f)
{
    __FLOCK(f);
    int rv = f->flags & F_ERR;
    __FUNLOCK(f);
    return rv;
}
