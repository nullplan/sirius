#include <unistd.h>
#include "syscall.h"

int pipe(int pfd[2])
{
    #ifdef SYS_pipe
    return syscall(SYS_pipe, pfd);
    #else
    return syscall(SYS_pipe2, pfd, 0);
    #endif
}
