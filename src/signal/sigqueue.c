#include <signal.h>
#include "syscall.h"

int sigqueue(pid_t pid, int sig, union sigval val)
{
    siginfo_t si;
    si.si_signo = sig;
    si.si_code = SI_QUEUE;
    si.si_pid = __syscall(SYS_getpid);
    si.si_uid = __syscall(SYS_getuid);
    si.si_value = val;
    return syscall(SYS_rt_sigqueueinfo, pid, sig, &si);
}
