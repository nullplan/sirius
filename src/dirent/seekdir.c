#include <dirent.h>
#include <unistd.h>

void seekdir(DIR *d, long off)
{
    __lock(&d->lock);
    d->pos = d->end = d->buf;
    lseek(d->fd, off, SEEK_SET);
    __unlock(&d->lock);
}
