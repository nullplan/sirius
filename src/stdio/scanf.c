#include <stdio.h>
#include <stdarg.h>

int scanf(const char *restrict fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int rv = vfscanf(stdin, fmt, ap);
    va_end(ap);
    return rv;
}
