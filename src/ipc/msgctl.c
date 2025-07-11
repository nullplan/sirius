#include <sys/msg.h>
#include "syscall.h"

int msgctl(int msqid, int cmd, struct msqid_ds *buf)
{
    #ifdef SYS_ipc
    return syscall(SYS_ipc, 14, msqid, cmd | IPC_64, 0, buf);
    #else
    return syscall(SYS_msgctl, msqid, cmd, buf);
    #endif
}
