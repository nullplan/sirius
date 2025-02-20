#include <sched.h>
#include "syscall.h"

int sched_get_priority_min(int pol)
{
    return syscall(SYS_sched_get_priority_min, pol);
}
