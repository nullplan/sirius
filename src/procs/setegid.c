#include <unistd.h>
#include "syscall.h"

int setegid(gid_t gid)
{
    return __setxid(SYS_setresgid, -1, gid, -1);
}
