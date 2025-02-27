#include <unistd.h>
#include "syscall.h"

int dup2(int ofd, int nfd)
{
    #ifdef SYS_dup2
    return syscall(SYS_dup2, ofd, nfd);
    #else
    return ofd == nfd? ofd : syscall(SYS_dup3, ofd, nfd, 0);
    #endif
}
