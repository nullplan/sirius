#include <unistd.h>
#include <sys/resource.h>
#include <errno.h>
#include "syscall.h"

int nice(int incr)
{
    /* use getpriority syscall to avoid handling errno overmuch. */
    int kprio = syscall(SYS_getpriority, PRIO_PROCESS, 0);
    if (kprio == -1) return -1;

    /* avoid overflow first */
    if (incr < -100) incr = -100;
    if (incr > 100) incr = 100;

    /* kprio has opposite orientation of uprio, so subtract the increment */
    kprio -= incr;
    /* clamp kprio to correct values. */
    if (kprio < 1) kprio = 1;
    else if (kprio > 40) kprio = 40;

    int uprio = NZERO - kprio;
    if (setpriority(PRIO_PROCESS, 0, uprio))
        return -1;
    return uprio;
}
