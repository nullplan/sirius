#include <sys/socket.h>
#include "syscall.h"

ssize_t send(int sk, const void *buf, size_t len, int flags)
{
    #ifdef SYS_send
    ssize_t rv = __syscall(SYS_send, sk, buf, len, flags);
    #else
    ssize_t rv = __syscall(SYS_sendto, sk, buf, len, flags, 0, 0);
    #endif
    #ifdef SYS_socketcall
    if (rv == -ENOSYS)
        rv = __syscall(SYS_socketcall, SYS_SEND, ((long[]){sk, (long)buf, len, flags}));
    #endif
    return __syscall_ret(rv);
}
