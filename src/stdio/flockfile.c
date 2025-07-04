#include <stdio.h>
#include "cpu.h"

void flockfile(FILE *f)
{
    if (!ftrylockfile(f)) return;
    if (__flockfile(f))
        __register_locked_file(f);
    else
        a_crash(); /* I think this case is impossible. */
}
