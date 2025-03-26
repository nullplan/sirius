#include <unistd.h>
#include "syscall.h"

int setresgid(gid_t r, gid_t e, gid_t s)
{
    return __setxid(SYS_setresgid, r, e, s);
}
