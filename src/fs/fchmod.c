#include <sys/stat.h>
#include "syscall.h"

int fchmod(int fd, mode_t m)
{
    return syscall(SYS_fchmod, fd, m);
}
