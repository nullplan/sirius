#include <termios.h>
#include <sys/ioctl.h>

int tcflush(int fd, int q)
{
    return ioctl(fd, TCFLSH, q);
}
