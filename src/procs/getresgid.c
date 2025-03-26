#include <unistd.h>
#include "syscall.h"

int getresgid(gid_t *restrict r, gid_t *restrict e, gid_t *restrict s)
{
    return syscall(SYS_getresgid, r, e, s);
}
