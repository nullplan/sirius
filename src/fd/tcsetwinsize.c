#include <termios.h>
#include <sys/ioctl.h>

int tcsetwinsize(int fd, const struct winsize *wsz)
{
    return ioctl(fd, TIOCGWINSZ, wsz);
}
