#include <sys/shm.h>
#include "syscall.h"

int shmget(key_t k, size_t sz, int flg)
{
    #ifdef SYS_ipc
    return syscall(SYS_ipc, 23, k, sz, flg);
    #else
    return syscall(SYS_shmget, k, sz, flg);
    #endif
}
