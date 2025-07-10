#include <stdlib.h>
#include <sys/ioctl.h>

int unlockpt(int fd)
{
    return ioctl(fd, TIOCSPTLCK, &(int){0});
}
