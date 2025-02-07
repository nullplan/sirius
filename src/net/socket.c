#include <sys/socket.h>
#include <fcntl.h>
#include "syscall.h"

/* On socketcall architectures, the socket syscall is newer than
 * socket flags support.
 * So if the initial socket creation fails because of socket flags,
 * we can immediately fall back to socketcall.
 */
int socket(int domain, int type, int proto)
{
    int rv = __syscall(SYS_socket, domain, type, proto);
    #ifdef SYS_socketcall
    if (rv == -ENOSYS)
        rv = __syscall(SYS_socketcall, SYS_SOCKET, ((long[]){domain, type, proto}));
    #endif
    if (rv == -EINVAL && (type & (SOCK_CLOEXEC|SOCK_NONBLOCK))) {
        #ifdef SYS_socketcall
        rv = __syscall(SYS_socketcall, SYS_SOCKET, ((long[]){domain, type & ~(SOCK_CLOEXEC|SOCK_NONBLOCK), proto}));
        #else
        rv = __syscall(SYS_socket, domain, type & ~(SOCK_CLOEXEC|SOCK_NONBLOCK), proto);
        #endif
        if (rv >= 0) {
            if (type & SOCK_CLOEXEC) __syscall(SYS_fcntl, rv, F_SETFD, FD_CLOEXEC);
            if (type & SOCK_NONBLOCK) __syscall(SYS_fcntl, rv, F_SETFL, O_NONBLOCK);
        }
    }
    return __syscall_ret(rv);
}
