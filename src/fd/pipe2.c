#include <unistd.h>
#include <fcntl.h>
#include "syscall.h"

int pipe2(int pfd[2], int flg)
{
    int rv;
    /* none of the supported architectures has the weird two-retval SYS_pipe variant. */
    #ifdef SYS_pipe
    if (!flg)
        rv = __syscall(SYS_pipe, pfd);
    else
    #endif
        rv = __syscall(SYS_pipe2, pfd, flg);
    #ifdef SYS_pipe
    if (rv == -ENOSYS) {
        if (flg & ~(O_CLOEXEC|O_NONBLOCK))
            rv = -EINVAL;
        else {
            rv = __syscall(SYS_pipe, pfd);
            if (!rv) {
                if (flg & O_CLOEXEC) {
                    __syscall(SYS_fcntl, pfd[0], F_SETFD, FD_CLOEXEC);
                    __syscall(SYS_fcntl, pfd[1], F_SETFD, FD_CLOEXEC);
                }
                if (flg & O_NONBLOCK) {
                    __syscall(SYS_fcntl, pfd[0], F_SETFL, O_NONBLOCK);
                    __syscall(SYS_fcntl, pfd[1], F_SETFL, O_NONBLOCK);
                }
            }
        }
    }
    #endif
    return __syscall_ret(rv);
}
