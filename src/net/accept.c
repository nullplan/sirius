#include <sys/socket.h>
#include "syscall.h"

int accept(int sk, struct sockaddr *restrict sa, socklen_t *restrict slen)
{
    return accept4(sk, sa, slen, 0);
}
