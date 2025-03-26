#include <unistd.h>
#include "syscall.h"

int setresuid(uid_t r, uid_t e, uid_t s)
{
    return __setxid(SYS_setresuid, r, e, s);
}
