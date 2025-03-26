#include <unistd.h>
#include "syscall.h"

int setregid(gid_t r, gid_t e)
{
    return __setxid(SYS_setregid, r, e, -1);
}
