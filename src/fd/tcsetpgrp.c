#include <unistd.h>
#include <sys/ioctl.h>
#include "syscall.h"

int tcsetpgrp(int fd, pid_t pg)
{
    return syscall(SYS_ioctl, fd, TIOCSPGRP, &pg);
}
