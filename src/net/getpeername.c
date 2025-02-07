#include <sys/socket.h>
#include "syscall.h"

int getpeername(int sk, struct sockaddr *restrict sa, socklen_t *restrict sl)
{
    int rv = __syscall(SYS_getpeername, sk, sa, sl);
    #ifdef SYS_socketcall
    if (rv == -ENOSYS)
        rv = __syscall(SYS_socketcall, SYS_GETPEERNAME, ((long[]){sk, (long)sa, (long)sl}));
    #endif
    return __syscall_ret(rv);
}
