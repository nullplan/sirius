#include <stdio.h>

int fgetc(FILE *f)
{
    int rv;
    __FLOCK(f);
    rv = getc_unlocked(f);
    __FUNLOCK(f);
    return rv;
}
weak_alias(getc, fgetc);
