#include <time.h>
#include "syscall.h"

int clock_nanosleep(clockid_t clk, int flags, const struct timespec *ts, struct timespec *rem)
{
    return syscall_cp(SYS_clock_nanosleep, clk, flags, ts, rem);
}
