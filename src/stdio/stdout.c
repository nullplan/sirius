#include <stdio.h>
#include <sys/ioctl.h>
#include "syscall.h"

static unsigned char buf[UNGET+BUFSIZ];
static size_t stdout_write(FILE *f, const void *buf, size_t len)
{
    f->write = __stdio_write;
    struct winsize wsz;
    if (!(f->flags & F_SETBUF) && __syscall(SYS_ioctl, f->fd, TIOCGWINSZ, &wsz) < 0)
        f->lbf = -1;
    return __stdio_write(f, buf, len);
}

static FILE f = {
    .fd = 1,
    .flags = F_NORD | F_PERM,
    .buf = buf + UNGET,
    .buf_size = BUFSIZ,
    .lbf = '\n',
    .write = stdout_write,
    .seek = __stdio_seek,
    .close = __stdio_close,
    .lock = -1,
};
hidden FILE *const __stdout_used = &f;
weak_alias(stdout, __stdout_used);
