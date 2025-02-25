#include <pty.h>
#include <unistd.h>

pid_t forkpty(int *m, char *name, const struct termios *tio, const struct winsize *win)
{
    int s;
    int rv = openpty(m, &s, name, tio, win);
    if (rv) return -1;

    rv = fork();
    if (rv == -1) {
        close(*m);
        close(s);
        return -1;
    }

    if (!rv) {
        if (login_tty(s))
            _Exit(127);
    } else close(s);
    return rv;
}
