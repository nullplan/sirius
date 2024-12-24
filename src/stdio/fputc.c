#include <stdio.h>

int fputc(int c, FILE *f)
{
    int rv;
    __FLOCK(f);
    rv = putc_unlocked(c, f);
    __FUNLOCK(f);
    return rv;
}
