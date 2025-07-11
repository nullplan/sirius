#include <sys/shm.h>
#include <errno.h>
#include "syscall.h"

void *shmat(int id, const void *addr, int flg)
{
    #ifdef SYS_ipc
    void *raddr;
    long rv = __syscall(SYS_ipc, 21, id, flg, &raddr, addr);
    if (rv) {
        errno = -rv;
        raddr = SHM_FAILED;
    }
    return raddr;
    #else
    return (void *)syscall(SYS_shmat, id, addr, flg);
    #endif
}
