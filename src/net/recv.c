#include <sys/socket.h>
#include "syscall.h"

ssize_t recv(int sk, void *buf, size_t len, int flags)
{
    #ifdef SYS_recv
    ssize_t rv = __syscall(SYS_recv, sk, buf, len, flags);
    #else
    ssize_t rv = __syscall(SYS_recvfrom, sk, buf, len, flags, 0, 0);
    #endif
    #ifdef SYS_socketcall
    if (rv == -ENOSYS)
        rv = __syscall(SYS_socketcall, SYS_RECV, ((long[]){sk, (long)buf, len, flags}));
    #endif
    return __syscall_ret(rv);
}
