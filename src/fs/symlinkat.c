#include <unistd.h>
#include "syscall.h"

int symlinkat(const char *ofn, int nfd, const char *nfn)
{
    int rv;
    #ifdef SYS_symlink
    if (nfd == -100 || *nfn == '/')
        rv = __syscall(SYS_symlink, ofn, nfn);
    else
    #endif
        rv = __syscall(SYS_symlinkat, ofn, nfd, nfn);
    return __syscall_ret(rv);
}
