#include <sys/stat.h>
#include "syscall.h"

int chmod(const char *nm, mode_t m)
{
#ifdef SYS_chmod
    return syscall(SYS_chmod, nm, m);
#else
    return syscall(SYS_fchmodat, -100, nm, m, 0);
#endif
}
