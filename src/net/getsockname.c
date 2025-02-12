#include <sys/socket.h>
#include "syscall.h"

int getsockname(int sk, struct sockaddr *restrict sa, socklen_t *restrict sl)
{
    int rv = __syscall(SYS_getsockname, sk, sa, sl);
    #ifdef SYS_socketcall
    if (rv == -ENOSYS)
        rv = __syscall(SYS_socketcall, SYS_GETSOCKNAME, ((long[]){sk, (long)sa, (long)sl}));
    #endif
    return __syscall_ret(rv);
}
