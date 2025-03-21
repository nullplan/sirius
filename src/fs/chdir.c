#include <unistd.h>
#include "syscall.h"

int chdir(const char *dir)
{
    return syscall(SYS_chdir, dir);
}
