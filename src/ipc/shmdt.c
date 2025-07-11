#include <sys/shm.h>
#include "syscall.h"

int shmdt(const void *addr)
{
    #ifdef SYS_ipc
    return syscall(SYS_ipc, 22, 0, 0, addr);
    #else
    return syscall(SYS_shmdt, addr);
    #endif
}
