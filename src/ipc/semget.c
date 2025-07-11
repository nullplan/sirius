#include <sys/sem.h>
#include "syscall.h"

int semget(key_t sem, int n, int flg)
{
    #ifdef SYS_ipc
    return syscall(SYS_ipc, 2, sem, n, flg);
    #else
    return syscall(SYS_semget, sem, n, flg);
    #endif
}
