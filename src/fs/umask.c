#include <sys/stat.h>
#include "syscall.h"

mode_t umask(mode_t m)
{
    return syscall(SYS_umask, m);
}
