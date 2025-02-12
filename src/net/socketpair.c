#include <sys/socket.h>
#include <fcntl.h>
#include "syscall.h"

int socketpair(int domain, int type, int proto, int sk[2])
{
    int rv = __syscall(SYS_socketpair, domain, type, proto, sk);
    #ifdef SYS_socketcall
    if (rv == -ENOSYS)
        rv = __syscall(SYS_socketcall, SYS_SOCKETPAIR, ((long[]){domain, type, proto, (long)sk}));
    #endif
    if (rv == -EINVAL && (type & (SOCK_CLOEXEC|SOCK_NONBLOCK))) {
        #ifdef SYS_socketcall
        rv = __syscall(SYS_socketcall, SYS_SOCKETPAIR, ((long[]){domain, type & ~(SOCK_CLOEXEC|SOCK_NONBLOCK), proto, (long)sk}));
        #else
        rv = __syscall(SYS_socketpair, domain, type & ~(SOCK_CLOEXEC|SOCK_NONBLOCK), proto, sk);
        #endif
        if (!rv) {
            if (type & SOCK_CLOEXEC) {
                __syscall(SYS_fcntl, sk[0], F_SETFD, FD_CLOEXEC);
                __syscall(SYS_fcntl, sk[1], F_SETFD, FD_CLOEXEC);
            }
            if (type & SOCK_NONBLOCK) {
                __syscall(SYS_fcntl, sk[0], F_SETFL, O_NONBLOCK);
                __syscall(SYS_fcntl, sk[1], F_SETFL, O_NONBLOCK);
            }
        }
    }
    return __syscall_ret(rv);
}
