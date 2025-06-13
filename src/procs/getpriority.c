#include <sys/resource.h>
#include <limits.h>
#include "syscall.h"

int getpriority(int which, id_t who)
{
    int kprio = syscall(SYS_getpriority, which, who);
    if (kprio == -1) return -1;
    return NZERO - kprio;
}
