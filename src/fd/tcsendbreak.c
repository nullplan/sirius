#include <termios.h>
#include <sys/ioctl.h>

int tcsendbreak(int fd, int d)
{
    return ioctl(fd, TCSBRKP, d);
}
