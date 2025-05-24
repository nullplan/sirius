#include <termios.h>
#include <sys/ioctl.h>

int tcgetwinsize(int fd, struct winsize *wsz)
{
    return ioctl(fd, TIOCGWINSZ, wsz);
}
