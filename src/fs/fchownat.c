#include <unistd.h>
#include <fcntl.h>
#include "syscall.h"

int fchownat(int dfd, const char *path, uid_t uid, gid_t gid, int flags)
{
    int rv;
    #ifdef SYS_chown
    if (dfd == AT_FDCWD || *path == '/') {
        rv = __syscall(flags & AT_SYMLINK_NOFOLLOW? SYS_lchown : SYS_chown, path, uid, gid);
    } else
    #endif
    #ifdef SYS_fchown
    if (!*path && (flags & AT_EMPTY_PATH))
        rv = __syscall(SYS_fchown, dfd, uid, gid);
    else
    #endif
        rv = __syscall(SYS_fchownat, dfd, path, uid, gid);
    return __syscall_ret(rv);
}
