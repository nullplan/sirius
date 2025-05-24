#include <termios.h>
#include <sys/ioctl.h>

int tcflow(int fd, int act)
{
    return ioctl(fd, TCXONC, act);
}
