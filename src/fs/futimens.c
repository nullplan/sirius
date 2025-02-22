#include <sys/stat.h>
#include "syscall.h"

int futimens(int fd, const struct timespec tms[2])
{
    return utimensat(fd, 0, tms, 0);
}
