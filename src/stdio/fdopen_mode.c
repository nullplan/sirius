#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <syscall.h>
#include <fcntl.h>
#include "libc.h"

hidden FILE *__fdopen_flg(int fd, int flg)
{
    FILE *f = malloc(sizeof (FILE) + UNGET + BUFSIZ);
    if (!f) return 0;
    memset(f, 0, sizeof (FILE));
    f->fd = fd;
    if ((flg & O_ACCMODE) == O_RDONLY) f->flags = F_NOWR;
    if ((flg & O_ACCMODE) == O_WRONLY) f->flags = F_NORD;
    if (flg & O_CLOEXEC) __syscall(SYS_fcntl, fd, F_SETFD, FD_CLOEXEC);

    f->buf = (unsigned char *)f + sizeof (FILE) + UNGET;
    f->buf_size = BUFSIZ;
    struct winsize wsz;
    if (!__syscall(SYS_ioctl, fd, TIOCGWINSZ, &wsz))
        f->lbf = '\n';
    else
        f->lbf = -1;

    f->read = __stdio_read;
    f->write = __stdio_write;
    f->seek = __stdio_seek;
    f->close = __stdio_close;

    f->lock = __threaded - 1;

    FILE *head = __ofl_lock();
    f->next = head;
    if (head) head->prev = f;
    __ofl_unlock(f);
    return f;
}
