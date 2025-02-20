#include <sched.h>
#include "syscall.h"

int sched_get_rr_interval(pid_t p, struct timespec *ts)
{
    return syscall(SYS_sched_rr_get_interval, p, ts);
}
