#include <sys/socket.h>
#include "syscall.h"

int getsockopt(int sk, int level, int opt, void *restrict arg, socklen_t *restrict len)
{
    int rv = __syscall(SYS_getsockopt, sk, level, opt, arg, len);
    #ifdef SYS_socketcall
    if (rv == -ENOSYS)
        rv = __syscall(SYS_socketcall, SYS_GETSOCKOPT, ((long[]){sk, level, opt, (long)arg, (long)len}));
    #endif
    return __syscall_ret(rv);
}
