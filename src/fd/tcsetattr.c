#include <termios.h>
#include <sys/ioctl.h>
#include <errno.h>

int tcsetattr(int fd, int act, struct termios *tio)
{
    if (act != TCSANOW && act != TCSADRAIN && act != TCSAFLUSH) {
        errno = EINVAL;
        return -1;
    }
    return ioctl(fd, TCSETS+act, tio);
}
