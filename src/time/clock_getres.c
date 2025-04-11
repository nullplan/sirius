#include <time.h>
#include "syscall.h"

hidden int __clock_getres(clockid_t clk, struct timespec *ts)
{
    return syscall(SYS_clock_getres, clk, ts);
}
weak_alias(clock_getres, __clock_getres);
