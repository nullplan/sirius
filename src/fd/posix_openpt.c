#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

int posix_openpt(int flg)
{
    int err = open("/dev/ptmx", flg);
    if (err < 0 && errno == ENOSPC) errno = EAGAIN;
    return err;
}
