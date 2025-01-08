#include <signal.h>
#include "syscall.h"

int raise(int sig)
{
    sigset_t ss;
    __block_usr_sigs(&ss);
    int rv = syscall(SYS_tkill, __pthread_self()->tid, sig);
    __restore_sigs(&ss);
    return rv;
}
