#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "syscall.h"

int faccessat(int dfd, const char *name, int mode, int flags)
{
#ifdef SYS_access
    if ((dfd == -100 || *name == '/') && !flags)
        return syscall(SYS_access, name);
#endif
    int rv;
#ifdef SYS_faccessat
    if (!flags)
        rv = __syscall(SYS_faccessat, dfd, name, mode);
    else
#endif
        rv = __syscall(SYS_faccessat2, dfd, name, mode, flags);
    if (rv == -ENOSYS) {
        if ((flags & ~(AT_EACCESS | AT_SYMLINK_NOFOLLOW)) || (mode > R_OK))
            rv = -EINVAL;
        else {
            struct stat st;
            if (fstatat(dfd, name, &st, flags & AT_SYMLINK_NOFOLLOW)) return -1;
            if (mode == F_OK) return 0;
            uid_t uid = flags & AT_EACCESS? geteuid() : getuid();
            gid_t gid = flags & AT_EACCESS? getegid() : getgid();
            int m = st.st_mode;
            if (uid == st.st_uid) m >>= 6;
            else if (gid == st.st_gid) m >>= 3;
            rv = mode & ~m? -EACCES : 0;
        }
    }
    return __syscall_ret(rv);
}
