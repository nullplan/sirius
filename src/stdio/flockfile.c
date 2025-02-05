#include <stdio.h>

void flockfile(FILE *f)
{
    if (!ftrylockfile(f)) return;
    if (__flockfile(f))
        __register_locked_file(f);
    else
        f->count++;
}
