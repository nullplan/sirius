#include <sys/shm.h>
#include "syscall.h"

int shmctl(int id, int cmd, struct shmid_ds *buf)
{
    #ifdef SYS_ipc
    return syscall(SYS_ipc, 24, id, cmd | IPC_64, 0, buf);
    #else
    return syscall(SYS_shmctl, id, cmd, buf);
    #endif
}
