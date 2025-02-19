#include <stdio.h>
#include <fcntl.h>
#include "syscall.h"

int renameat(int ofd, const char *ofn, int nfd, const char *nfn)
{
    int rv;
#ifdef SYS_rename
    if ((ofd == AT_FDCWD || *ofn == '/')
            && (nfd == AT_FDCWD || *nfn == '/'))
    {
        rv = __syscall(SYS_rename, ofn, nfn);
    }
    else
#endif
#ifdef SYS_renameat
    rv = __syscall(SYS_renameat, ofd, ofn, nfd, nfn);
#else
    rv = __syscall(SYS_renameat2, ofd, ofn, nfd, nfn, 0);
#endif
    return __syscall_ret(rv);
}
