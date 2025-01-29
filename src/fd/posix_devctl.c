#include <devctl.h>
#include "syscall.h"

int posix_devctl(int fd, int cmd, void *restrict ptr, size_t sz, int *restrict info)
{
    (void) sz;  /* XXX: Validate size for know ioctls? */
    (void) info;
    return -__syscall(SYS_ioctl, fd, cmd, ptr);
}
