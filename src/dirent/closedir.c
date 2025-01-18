#include <dirent.h>
#include <stdlib.h>
#include "syscall.h"

int closedir(DIR *d)
{
    __syscall(SYS_close, d->fd);
    free(d);
    return 0;
}
