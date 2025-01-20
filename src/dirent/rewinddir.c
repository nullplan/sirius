#include <dirent.h>
#include <unistd.h>

void rewinddir(DIR *d)
{
    __lock(&d->lock);
    d->pos = d->end = d->buf;
    lseek(d->fd, 0, SEEK_SET);
    __unlock(&d->lock);
}
