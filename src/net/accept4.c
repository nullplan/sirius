#include <sys/socket.h>
#include <fcntl.h>
#include "syscall.h"

/* History lesson first: SYS_accept4 was introduced at the same time as SYS_ACCEPT4.
 * So a kernel has either both or neither.
 * Before that, there was SYS_accept, and before that SYS_socketcall.
 *
 * Some architectures lack SYS_accept or SYS_socketcall. So they must be treated as optional.
 *
 * So, what syscall chain do I want here?
 * if (!flg)
 *      SYS_accept -> SYS_socketcall
 * else
 *      SYS_accept4 -> SYS_accept + flg emulation -> SYS_socketcall + flg emulation
 */
int accept4(int sk, struct sockaddr *restrict sa, socklen_t *restrict slen, int flg)
{
    int rv;
    #ifdef SYS_accept
    if (!flg)
        rv = __syscall_cp(SYS_accept, sk, sa, slen);
    else
    #endif
        rv = __syscall_cp(SYS_accept4, sk, sa, slen, flg);
    #if defined SYS_accept || defined SYS_socketcall
    if (rv == -ENOSYS) {
        if (flg & ~(SOCK_CLOEXEC | SOCK_NONBLOCK))
            rv = -EINVAL;
        #ifdef SYS_accept
        if (rv == -ENOSYS && flg)
            rv = __syscall_cp(SYS_accept, sk, sa, slen);
        #endif
        #ifdef SYS_socketcall
        if (rv == -ENOSYS)
            rv = __syscall_cp(SYS_socketcall, SYS_ACCEPT, ((long[]){sk, (long)sa, (long)slen}));
        #endif
        if (rv >= 0) {
            if (flg & SOCK_CLOEXEC) __syscall(SYS_fcntl, rv, F_SETFD, FD_CLOEXEC);
            if (flg & SOCK_NONBLOCK) __syscall(SYS_fcntl, rv, F_SETFL, O_NONBLOCK);
        }
    }
    #endif
    return __syscall_ret(rv);
}
