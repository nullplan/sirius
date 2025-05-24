#include <termios.h>
#include <sys/ioctl.h>

pid_t tcgetsid(int fd)
{
    pid_t rv;
    if (ioctl(fd, TIOCGSID, &rv)) return -1;
    return rv;
}
