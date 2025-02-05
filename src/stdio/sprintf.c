#include <stdio.h>
#include <stdarg.h>

int sprintf(char *restrict buf, const char *restrict fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int rv = vsprintf(buf, fmt, ap);
    va_end(ap);
    return rv;
}
