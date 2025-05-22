#include <sys/wait.h>
#include "syscall.h"

pid_t wait(int *st)
{
    return syscall_cp(SYS_wait4, -1, st, 0, 0);
}
