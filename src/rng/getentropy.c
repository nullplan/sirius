#include <unistd.h>
#include <limits.h>
#include "syscall.h"

int getentropy(void *buf, size_t len)
{
    int rv;
    if (len > GETENTROPY_MAX)
        rv = -EINVAL;
    else {
        char *pos = buf;
        rv = 0;
        while (len) {
            rv = __syscall(SYS_getrandom, pos, len, 2);
            if (rv == -EINTR) continue;
            if (rv < 0) break;
            pos += rv;
            len -= rv;
            rv = 0;
        }
    }
    return __syscall_ret(rv);
}
