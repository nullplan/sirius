#include <stdio.h>
#include <stdarg.h>

int fscanf(FILE *restrict f, const char *restrict fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int rv = vfscanf(f, fmt, ap);
    va_end(ap);
    return rv;
}
