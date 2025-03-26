#include <unistd.h>
#include "syscall.h"

char *ttyname(int fd)
{
    static char buf[TTY_NAME_MAX];
    return ttyname_r(fd, buf, sizeof buf)? 0 : buf;
}
