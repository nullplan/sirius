#include <sys/stat.h>
#include <fcntl.h>
#include "syscall.h"

int chmod(const char *nm, mode_t m)
{
    return fchmodat(AT_FDCWD, nm, m, 0);
}
