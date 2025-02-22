#include <sys/stat.h>
#include "syscall.h"

int mknod(const char *name, mode_t mode, dev_t dev)
{
    return mknodat(-100, name, mode, dev);
}
