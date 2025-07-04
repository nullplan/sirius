#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <string.h>

DIR *fdopendir(int fd)
{
    struct stat st;

    if (fstat(fd, &st) < 0) return 0;
    if (!S_ISDIR(st.st_mode)) {
        errno = ENOTDIR;
        return 0;
    }
    int m = fcntl(fd, F_GETFL);
    if (m < 0) return 0;
    if ((m & O_ACCMODE) != O_RDONLY && (m & O_ACCMODE) != O_RDWR) {
        errno = EBADF;
        return 0;
    }

    DIR *d = malloc(sizeof (struct __dirstream) + DIRBUFSIZ);
    if (d) {
        memset(d, 0, sizeof (struct __dirstream));
        d->fd = fd;
        d->pos = d->end = d->buf;
    }
    return d;
}
