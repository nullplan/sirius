#include <sys/wait.h>
#include "syscall.h"

int waitid(idtype_t t, id_t id, siginfo_t *si, int opt)
{
    return syscall_cp(SYS_waitid, t, id, si, opt);
}
