#include <stdio.h>

void clearerr(FILE *f)
{
    __FLOCK(f);
    f->flags &= ~(F_ERR|F_EOF);
    __FUNLOCK(f);
}
