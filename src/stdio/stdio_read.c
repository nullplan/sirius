#include <stdio.h>
#include <syscall.h>
#include <sys/uio.h>

hidden ssize_t __stdio_read(FILE *f, void *buf, size_t len)
{
    struct iovec iov[2] = {
        {.iov_base = buf, .iov_len = len - !!f->buf_size},
        {.iov_base = f->buf, .iov_len = f->buf_size},
    };
    struct iovec *pio = iov;
    int iovcnt = 2;

    if (!pio->iov_len) {
        pio++;
        iovcnt--;
    }
    f->pos = f->end = f->buf;
    ssize_t rd = syscall(SYS_readv, f->fd, pio, iovcnt);
    if (rd <= 0) {
        if (rd < 0)
            f->flags |= F_ERR;
        else
            f->flags |= F_EOF;
        return -1;
    }
    if (rd >= len) {
        f->end += rd - len + 1;
        ((unsigned char *)buf)[len - 1] = *f->pos++;
        return len;
    }
    return rd;
}
