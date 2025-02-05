#include <stdio.h>
#include <stdarg.h>

int sscanf(const char *restrict s, const char *restrict fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int rv = vsscanf(s, fmt, ap);
    va_end(ap);
    return rv;
}
