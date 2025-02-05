#include <stdio.h>

int vdprintf(int fd, const char *restrict fmt, va_list ap)
{
    unsigned char buffer;
    FILE f;
    f.fd = fd;
    f.flags = F_NORD;
    f.buf = &buffer;
    f.buf_size = 0;
    f.dir = 0;
    f.lbf = -1;
    f.write = __stdio_write;
    f.lock = -1;
    return vfprintf(&f, fmt, ap);
}
