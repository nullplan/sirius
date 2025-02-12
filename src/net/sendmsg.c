#include <sys/socket.h>
#include "syscall.h"

ssize_t sendmsg(int sk, const struct msghdr *msg, int flags)
{
    ssize_t rv = __syscall(SYS_sendmsg, sk, msg, flags);
    #ifdef SYS_socketcall
    if (rv == -ENOSYS)
        rv = __syscall(SYS_socketcall, SYS_SENDMSG, ((long[]){sk, (long)msg, flags}));
    #endif
    return __syscall_ret(rv);
}
