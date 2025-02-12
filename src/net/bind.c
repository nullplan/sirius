#include <sys/socket.h>
#include "syscall.h"

int bind(int sk, const struct sockaddr *sa, socklen_t slen)
{
    int rv = __syscall(SYS_bind, sk, sa, slen);
    #ifdef SYS_socketcall
    if (rv == -ENOSYS)
        rv = __syscall(SYS_socketcall, SYS_BIND, ((long[]){sk, (long)sa, (long)slen}));
    #endif
    return __syscall_ret(rv);
}
