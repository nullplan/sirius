#include <stdlib.h>
#include <errno.h>

char *ptsname(int fd)
{
    static char buf[TTY_NAME_MAX];
    char *rv = buf;
    int err = ptsname_r(fd, buf, sizeof buf);
    if (err) {
        errno = err;
        rv = 0;
    }
    return rv;
}
