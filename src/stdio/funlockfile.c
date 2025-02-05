#include <stdio.h>
#include "pthread.h"

void funlockfile(FILE *f)
{
    if (f->count)
        f->count--;
    else {
        if (f->lnext) f->lnext->lprev = f->lprev;
        if (f->lprev) f->lprev->lnext = f->lnext;
        else __pthread_self()->locked_files = f->lnext;
        __funlockfile(f);
    }
}
