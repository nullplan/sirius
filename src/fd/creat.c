#include <fcntl.h>

int creat(const char *name, mode_t mode)
{
    return openat(AT_FDCWD, name, O_WRONLY|O_CREAT|O_TRUNC, mode);
}
