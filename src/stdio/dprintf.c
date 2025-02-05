#include <stdio.h>
#include <stdarg.h>

int dprintf(int fd, const char *restrict fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int rv = vdprintf(fd, fmt, ap);
    va_end(ap);
    return rv;
}
