#include <unistd.h>
#include "syscall.h"

int chown(const char *file, uid_t uid, gid_t gid)
{
    #ifdef SYS_chown
    return syscall(SYS_chown, file, uid, gid);
    #else
    return syscall(SYS_fchownat, -100, file, uid, gid);
    #endif
}
