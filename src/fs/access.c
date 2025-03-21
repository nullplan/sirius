#include <unistd.h>
#include <fcntl.h>

int access(const char *file, int mode)
{
    return faccessat(AT_FDCWD, file, mode, 0);
}
