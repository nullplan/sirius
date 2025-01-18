#include <dirent.h>

int dirfd(DIR *d)
{
    return d->fd;
}
