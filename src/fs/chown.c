#include <unistd.h>
#include <fcntl.h>

int chown(const char *file, uid_t uid, gid_t gid)
{
    return fchownat(AT_FDCWD, file, uid, gid, 0);
}
