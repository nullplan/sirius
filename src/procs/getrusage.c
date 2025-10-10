#include <sys/resource.h>
#include "syscall.h"

int getrusage(int who, struct rusage *ru)
{
    int rv = syscall(SYS_getrusage, who, ru);
    if (!rv)
    {
        ru->ru_utime.tv_sec = ru->__ru_utime_secs;
        ru->ru_utime.tv_usec = ru->__ru_utime_usecs;
        ru->ru_stime.tv_sec = ru->__ru_stime_secs;
        ru->ru_stime.tv_usec = ru->__ru_stime_usecs;
    }
    return rv;
}
