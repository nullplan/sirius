#include <pty.h>
#include <unistd.h>
#include <sys/ioctl.h>

int login_tty(int fd)
{
    setsid();
    if (ioctl(fd, TIOCSCTTY, 0)) return -1;
    dup2(fd, 0);
    dup2(fd, 1);
    dup2(fd, 2);
    if (fd > 2) close(fd);
    return 0;
}
