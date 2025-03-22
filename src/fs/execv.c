#include <unistd.h>
#include <stdlib.h>

int execv(const char *file, char *const args[])
{
    return execve(file, args, __environ);
}
