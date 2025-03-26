#include <unistd.h>
#include "syscall.h"

int seteuid(uid_t uid)
{
    return __setxid(SYS_setresuid, -1, uid, -1);
}
