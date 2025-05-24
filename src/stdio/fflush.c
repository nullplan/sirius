#include <stdio.h>

static int fflush_single(FILE *f)
{
    int res = 0;
    __FLOCK(f);
    if (IS_READ(f) && !(f->flags & F_EOF) && f->pos != f->end) {
        off_t res = f->seek(f, f->pos - f->end, SEEK_CUR);
        if (res == -1) res = -1;
    }
    if (IS_WRITE(f) && f->pos != f->buf) {
        f->write(f, 0, 0);
        if (f->flags & F_ERR) res = -1;
    }
    f->dir = 0;
    __FUNLOCK(f);
    return res;
}

int fflush(FILE *f)
{
    if (f) return fflush_single(f);
    int res = 0;
    FILE *head = __ofl_lock();
    for (f = head; f; f = f->next)
        res |= fflush_single(f);
    __ofl_unlock(head);
    return res;
}
