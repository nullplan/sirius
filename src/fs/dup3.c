#include <unistd.h>
#include <fcntl.h>
#include "syscall.h"

int dup3(int ofd, int nfd, int flg)
{
    int rv;
    #ifdef SYS_dup2
    if (ofd == nfd || (flg & ~O_CLOEXEC))
        rv = -EINVAL;
    else if (!flg)
        rv = __syscall(SYS_dup2, ofd, nfd);
    else
    #endif
    {
        rv = __syscall(SYS_dup3, ofd, nfd, flg);

        #ifdef SYS_dup2
        if (rv == -ENOSYS) {
            rv = __syscall(SYS_dup2, ofd, nfd);
            if (rv >= 0)
                __syscall(SYS_fcntl, rv, F_SETFD, FD_CLOEXEC);
        }
        #endif
    }
    return __syscall_ret(rv);
}
