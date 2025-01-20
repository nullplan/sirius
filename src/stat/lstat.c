#include <sys/stat.h>
#include <fcntl.h>

int lstat(const char *restrict fn, struct stat *restrict st)
{
    return fstatat(AT_FDCWD, fn, st, AT_SYMLINK_NOFOLLOW);
}
