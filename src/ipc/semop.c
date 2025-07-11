#include <sys/sem.h>
#include "syscall.h"

int semop(int id, struct sembuf *buf, size_t n)
{
    #ifdef SYS_ipc
    return syscall(SYS_ipc, 1, id, n, 0, buf);
    #else
    return syscall(SYS_semop, id, buf, n);
    #endif
}
