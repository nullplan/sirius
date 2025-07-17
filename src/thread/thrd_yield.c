#include <threads.h>
#include "syscall.h"

void thrd_yield(void)
{
    __syscall(SYS_sched_yield);
}
