#include <signal.h>
#include "syscall.h"

int kill(pid_t pid, int sig)
{
    return __syscall(SYS_kill, pid, sig);
}
