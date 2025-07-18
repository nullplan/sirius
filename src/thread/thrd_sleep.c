#include <threads.h>
#include "syscall.h"

int thrd_sleep(const struct timespec *req, struct timespec *rem)
{
    return syscall(SYS_nanosleep, req, rem);
}
