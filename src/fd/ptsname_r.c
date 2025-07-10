#include <stdlib.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include "syscall.h"

int ptsname_r(int fd, char *buf, size_t len)
{
    int n;
    int err = -__syscall(SYS_ioctl, fd, TIOCGPTN, &n);
    if (err) return err;
    if (snprintf(buf, len, "/dev/pts/%d", n) >= len)
        err = ERANGE;
    return err;
}
