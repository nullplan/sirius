#include <unistd.h>
#include "syscall.h"

int setuid(uid_t uid)
{
    return __setxid(SYS_setuid, uid, 0, 0);
}
