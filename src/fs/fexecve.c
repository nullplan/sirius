#define _GNU_SOURCE
#include <unistd.h>
#include <fcntl.h>
#include "syscall.h"
#include "libc.h"

int fexecve(int fd, char *const args[], char *const env[])
{
    int rv = __syscall(SYS_execveat, fd, "", args, env, AT_EMPTY_PATH);
    if (rv == -ENOSYS)
    {
        char pfdname[PROCFD_LEN];
        char *p = __procfdname(fd, pfdname + sizeof pfdname);
        rv = __syscall(SYS_execve, pfdname, args, env);
        if (rv == -ENOENT) rv = -EBADF;
    }
    return __syscall_ret(rv);
}
