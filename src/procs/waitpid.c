#include <sys/wait.h>
#include "syscall.h"

pid_t waitpid(pid_t pid, int *status, int flags)
{
    #ifdef SYS_waitpid
    return syscall(SYS_waitpid, pid, status, flags);
    #else
    return syscall(SYS_wait4, pid, status, flags, 0);
    #endif
}
