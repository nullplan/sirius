#include <time.h>
#include "syscall.h"

int nanosleep(const struct timespec *ts, struct timespec *rem)
{
    return syscall_cp(SYS_nanosleep, ts, rem);
}
