#include <stdio.h>

int setvbuf(FILE *restrict f, char *restrict buf, int mode, size_t size)
{
    int rv = -1;
    __FLOCK(f);
    if (mode == _IONBF || mode == _IOLBF || mode == _IOFBF) {
        rv = 0;
        f->flags |= F_SETBUF;
        f->buf_size = mode == _IONBF? 0 : BUFSIZ;
        f->lbf = mode == _IOLBF? '\n' : -1;
    }
    __FUNLOCK(f);
    return rv;
}
