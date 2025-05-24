#include <sys/socket.h>
#include "syscall.h"

ssize_t recvfrom(int sk, void *restrict buf, size_t len, int flags, struct sockaddr *restrict sa, socklen_t *restrict sl)
{
    ssize_t rv = __syscall_cp(SYS_recvfrom, sk, buf, len, flags, sa, sl);
    #ifdef SYS_socketcall
    if (rv == -ENOSYS)
        rv = __syscall_cp(SYS_socketcall, SYS_RECVFROM, ((long[]){sk, (long)buf, len, flags, (long)sa, (long)sl}));
    #endif
    return __syscall_ret(rv);
}
