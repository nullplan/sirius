#include <dirent.h>
#include <stdlib.h>
#include <fcntl.h>
#include "syscall.h"

DIR *opendir(const char *name)
{
    int fd = open(name, O_RDONLY | O_DIRECTORY | O_CLOEXEC);
    if (fd == -1) return 0;

    DIR *d = calloc(sizeof (struct __dirstream) + DIRBUFSIZ, 1);
    if (!d) {
        __syscall(SYS_close, fd);
    } else {
        d->fd = fd;
        d->pos = d->end = d->buf;
    }
    return d;
}
