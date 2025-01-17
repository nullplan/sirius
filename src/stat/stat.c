#include <sys/stat.h>
#include <fcntl.h>

int stat(const char *restrict fn, struct stat *restrict st)
{
    return fstatat(AT_FDCWD, fn, st, 0);
}
