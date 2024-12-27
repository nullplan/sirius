#include <stdio.h>

int fileno(FILE *f)
{
    __FLOCK(f);
    int fd = f->fd;
    __FUNLOCK(f);
    return fd;
}
