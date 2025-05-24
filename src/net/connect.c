#include <sys/socket.h>
#include "syscall.h"

int connect(int sk, const struct sockaddr *sa, socklen_t sl)
{
    int rv = __syscall_cp(SYS_connect, sk, sa, sl);
    #ifdef SYS_socketcall
    if (rv == -ENOSYS)
        rv = __syscall_cp(SYS_socketcall, SYS_CONNECT, ((long[]){sk, (long)sa, sl}));
    #endif
    return __syscall_ret(rv);
}
