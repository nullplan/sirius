#include <sys/wait.h>
#include "syscall.h"

pid_t waitpid(pid_t pid, int *status, int flags)
{
    return syscall_cp(SYS_wait4, pid, status, flags, 0);
}
