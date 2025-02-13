#include <stdio.h>
#include <stdlib.h>

int fclose(FILE *f)
{
    __flockfile(f);
    int r = fflush(f);
    r |= f->close(f);


    FILE *head = __ofl_lock();
    if (f->next) f->next->prev = f->prev;
    if (f->prev) f->prev->next = f->next;
    else head = f->next;
    __ofl_unlock(head);

    if (!(f->flags & F_PERM))
        free(f);
    return r;
}
