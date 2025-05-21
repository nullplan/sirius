#include <sys/socket.h>
#include "syscall.h"

ssize_t sendto(int sk, const void *buf, size_t len, int flags, const struct sockaddr *sa, socklen_t sl)
{
    ssize_t rv = __syscall_cp(SYS_sendto, sk, buf, len, flags, sa, sl);
    #ifdef SYS_socketcall
    if (rv == -ENOSYS)
        rv = __syscall_cp(SYS_socketcall, SYS_SENDTO, ((long[]){sk, (long)buf, len, flags, (long)sa, sl}));
    #endif
    return __syscall_ret(rv);
}
