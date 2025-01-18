#include <dirent.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

int readdir_r(DIR *restrict d, struct dirent *restrict buf, struct dirent **restrict ret)
{
    int rv = 0;
    __lock(&d->lock);
    int errno_save = errno;
    errno = 0;
    struct dirent *de = readdir(d);
    *ret = 0;
    if (!de)
        rv = errno;
    else
        *ret = memcpy(buf, de, de->d_reclen);
    errno = errno_save;
    __unlock(&d->lock);
    return rv;
}
