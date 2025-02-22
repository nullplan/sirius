#include <sys/stat.h>
#include "syscall.h"

int mkfifoat(int dfd, const char *name, mode_t m)
{
    return mknodat(dfd, name, S_IFIFO | m, 0);
}
