#include <sys/ioctl.h>
#include <stdarg.h>
#include "syscall.h"

int ioctl(int fd, int cmd, ...)
{
    long arg;
    va_list ap;
    va_start(ap, cmd);
    arg = va_arg(ap, long);
    va_end(ap);
    return syscall(SYS_ioctl, fd, cmd, arg);
}
