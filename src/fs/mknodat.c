#include <sys/stat.h>
#include "syscall.h"

int mknodat(int dfd, const char *name, mode_t mode, dev_t dev)
{
#ifdef SYS_mknod
    if (dfd == -100 || *name == '/')
        return syscall(SYS_mknod, name, mode, dev);
    else
#endif
        return syscall(SYS_mknodat, dfd, name, mode, dev);
}
