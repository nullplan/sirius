#include <stdio.h>
#include <stdarg.h>

int snprintf(char *restrict s, size_t n, const char *restrict fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int rv = vsnprintf(s, n, fmt, ap);
    va_end(ap);
    return rv;
}
