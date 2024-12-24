#include <stdio.h>
#include <syscall.h>
#include <sys/uio.h>

hidden size_t __stdio_write(FILE *f, const void *buf, size_t len)
{
    struct iovec iov[2] = {
        {.iov_base = f->buf, .iov_len = f->pos - f->buf},
        {.iov_base = (void *)buf, .iov_len = len},
    };
    struct iovec *pio = iov;
    int iovlen = 2;
    size_t rv = 0;

    f->pos = f->buf;
    f->end = f->buf + f->buf_size;
    for (;;)
    {
        ssize_t wr;
        if (iovlen == 1)
            wr = syscall(SYS_write, f->fd, pio->iov_base, pio->iov_len);
        else
            wr = syscall(SYS_writev, f->fd, pio, iovlen);

        /* fast-track success */
        if ((iovlen == 1 && wr == pio->iov_len)
                || (iovlen == 2 && wr == iov[0].iov_len + iov[1].iov_len))
            return len;

        if (wr < 0) {
            f->flags |= F_ERR;
            return rv;
        }

        /* if we get here, it is a partial success. */
        if (wr >= pio->iov_len) {
            wr -= pio->iov_len;
            pio++;
            iovlen--;
        }

        rv += wr;
        pio->iov_base = (char *)pio->iov_base + wr;
        pio->iov_len -= wr;
    }
}
