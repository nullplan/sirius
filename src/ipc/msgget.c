#include <sys/msg.h>
#include "syscall.h"

int msgget(key_t k, int msgflg)
{
    #ifdef SYS_ipc
    return syscall(SYS_ipc, 13, k, msgflg);
    #else
    return syscall(SYS_msgget, k, msgflg);
    #endif
}
