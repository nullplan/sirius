#include <unistd.h>
#include "syscall.h"

int execve(const char *name, char *const args[], char *const env[])
{
    return syscall(SYS_execve, name, args, env);
}
