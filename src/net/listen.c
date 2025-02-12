#include <sys/socket.h>
#include "syscall.h"

int listen(int sk, int n)
{
    int rv = __syscall(SYS_listen, sk, n);
    #ifdef SYS_socketcall
    if (rv == -ENOSYS)
        rv = __syscall(SYS_socketcall, SYS_LISTEN, ((long[]){sk, n}));
    #endif
    return __syscall_ret(rv);
}
