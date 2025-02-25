#include <pty.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include "syscall.h"

int openpty(int *m, int *s, char *name, const struct termios *tio, const struct winsize *win)
{
    char buf[sizeof "/dev/pts/" + 3 * sizeof (int)];
    int mfd = open("/dev/ptmx", O_RDWR | O_NOCTTY);
    if (mfd == -1) return -1;

    int n = 0;
    if (ioctl(mfd, TIOCSPTLCK, &n) || ioctl(mfd, TIOCGPTN, &n)) {
        close(mfd);
        return -1;
    }

    if (!name) name = buf;
    snprintf(name, sizeof buf, "/dev/pts/%d", n);
    int sfd = open(name, O_RDWR | O_NOCTTY);
    if (sfd == -1) {
        close(mfd);
        return -1;
    }

    *m = mfd;
    *s = sfd;
    if (tio) ioctl(sfd, TCSETS, tio);
    if (win) ioctl(sfd, TIOCSWINSZ, win);
    return 0;
}
