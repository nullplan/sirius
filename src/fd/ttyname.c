#include <unistd.h>
#include "syscall.h"

char *ttyname(int fd)
{
    static char buf[TTY_NAME_MAX];
    int rv = ttyname_r(fd, buf, sizeof buf);
    if (rv) {
        errno = rv;
        return 0;
    }
    return buf;
}
