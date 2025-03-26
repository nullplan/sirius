#include <unistd.h>
#include "syscall.h"

int getresuid(uid_t *restrict r, uid_t *restrict e, uid_t *restrict s)
{
    return syscall(SYS_getresuid, r, e, s);
}
