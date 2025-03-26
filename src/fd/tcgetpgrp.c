#include <syscall.h>
#include <sys/ioctl.h>
#include "syscall.h"

pid_t tcgetpgrp(int fd)
{
    pid_t rv;
    if (syscall(SYS_ioctl, fd, TIOCGPGRP, &rv))
        rv = -1;
    return rv;
}
