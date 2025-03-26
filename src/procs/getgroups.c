#include <unistd.h>
#include "syscall.h"

int getgroups(int sz, gid_t list[])
{
    return syscall(SYS_getgroups, sz, list);
}
