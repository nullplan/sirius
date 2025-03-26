#include <unistd.h>
#include <fcntl.h>

int lchown(const char *fn, uid_t uid, gid_t gid)
{
    return fchownat(AT_FDCWD, fn, uid, gid, AT_SYMLINK_NOFOLLOW);
}
