#include <sys/stat.h>
#include "syscall.h"

int fchmodat(int dfd, const char *nm, mode_t m, int flags)
{
    return syscall(SYS_fchmodat, dfd, nm, m, flags);
}
