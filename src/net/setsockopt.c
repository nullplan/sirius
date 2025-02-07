#include <sys/socket.h>
#include "syscall.h"

int setsockopt(int sk, int level, int opt, const void *arg, socklen_t len)
{
    int rv = __syscall(SYS_setsockopt, sk, level, opt, arg, len);
    #ifdef SYS_socketcall
    if (rv == -ENOSYS)
        rv = __syscall(SYS_socketcall, SYS_SETSOCKOPT, ((long[]){sk, level, opt, (long)arg, len}));
    #endif
    return __syscall_ret(rv);
}
