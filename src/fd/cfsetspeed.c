#include <termios.h>
#include <errno.h>

int cfsetospeed(struct termios *tio, speed_t s)
{
    if (s & ~CBAUD) {
        errno = EINVAL;
        return -1;
    }

    tio->c_cflag = (tio->c_cflag & ~CBAUD) | s;
    return 0;
}

int cfsetispeed(struct termios *tio, speed_t s)
{
    if (!s) return 0;
    return cfsetospeed(tio, s);
}
