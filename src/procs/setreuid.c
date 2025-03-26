#include <unistd.h>
#include "syscall.h"

int setreuid(uid_t r, uid_t e)
{
    return __setxid(SYS_setreuid, r, e, -1);
}
