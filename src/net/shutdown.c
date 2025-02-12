#include <sys/socket.h>
#include "syscall.h"

int shutdown(int sk, int mode)
{
    int rv = __syscall(SYS_shutdown, sk, mode);
    #ifdef SYS_socketcall
    if (rv == -ENOSYS)
        rv = __syscall(SYS_socketcall, SYS_SHUTDOWN, ((long[]){sk, mode}));
    #endif
    return __syscall_ret(rv);
}
