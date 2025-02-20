#include <sched.h>
#include "syscall.h"

int sched_get_priority_max(int pol)
{
    return syscall(SYS_sched_get_priority_max, pol);
}
