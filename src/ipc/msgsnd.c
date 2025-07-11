#include <sys/msg.h>
#include "syscall.h"

int msgsnd(int q, const void *b, size_t len, int type)
{
    #ifdef SYS_ipc
    return syscall(SYS_ipc, 11, q, len, type, b);
    #else
    return syscall(SYS_msgsnd, q, b, len, type);
    #endif
}
