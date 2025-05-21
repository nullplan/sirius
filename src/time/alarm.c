#include <unistd.h>
#include <time.h>
#include "syscall.h"

unsigned alarm(unsigned t)
{
    #ifdef SYS_alarm
    return syscall(SYS_alarm, t);
    #else
    struct itimerspec old, new = {0};
    new.it_value.tv_sec = t;
    syscall(SYS_setitimer, ITIMER_REAL, &new, &old);
    /* We can only return integer seconds. If an alarm is active, we must
     * return non-zero. And we should probably do some rounding
     * otherwise.
     */
    if ((old.it_value.tv_sec == 0 && old.it_value.tv_nsec > 0)
            || old.it_value.tv_nsec >= 500000000)
        old.it_value.tv_sec++;
    return old.it_value.tv_sec;
    #endif
}
