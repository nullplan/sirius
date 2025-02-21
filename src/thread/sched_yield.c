#include <sched.h>
#include "syscall.h"

int sched_yield(void)
{
    return syscall(SYS_sched_yield);
}
