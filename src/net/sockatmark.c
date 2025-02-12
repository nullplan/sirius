#include <sys/socket.h>
#include <sys/ioctl.h>
#include "syscall.h"

int sockatmark(int sk)
{
    int atmark;
    int rv = syscall(SYS_ioctl, sk, SIOCATMARK, &atmark);
    if (!rv) rv = atmark;
    return rv;
}
