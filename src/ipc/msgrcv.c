#include <sys/msg.h>
#include "syscall.h"

ssize_t msgrcv(int q, void *b, size_t len, long type, int flg)
{
    #ifdef SYS_ipc
    return syscall(SYS_ipc, 12, q, len, flg, (&(long[]){(long)b, type}));
    #else
    return syscall(SYS_msgrcv, q, b, len, type, flg);
    #endif
}
