#include <sys/socket.h>
#include "syscall.h"

ssize_t recvmsg(int sk, struct msghdr *msg, int flags)
{
    ssize_t rv = __syscall_cp(SYS_recvmsg, sk, msg, flags);
    #ifdef SYS_socketcall
    if (rv == -ENOSYS)
        rv = __syscall_cp(SYS_socketcall, SYS_RECVMSG, ((long[]){sk, (long)msg, flags}));
    #endif
    return __syscall_ret(rv);
}
