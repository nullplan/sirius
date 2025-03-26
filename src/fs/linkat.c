#include <unistd.h>
#include <fcntl.h>
#include "syscall.h"

int linkat(int ofd, const char *oname, int nfd, const char *nname, int flags)
{
    int rv;
    #ifdef SYS_link
    if (!flags && (ofd == AT_FDCWD || *oname == '/') && (nfd == AT_FDCWD || *nname == '/'))
        rv = __syscall(SYS_link, oname, nname);
    else
    #endif
        rv = __syscall(SYS_linkat, ofd, oname, nfd, nname, flags);
    return __syscall_ret(rv);
}
