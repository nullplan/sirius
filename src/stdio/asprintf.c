#include <stdio.h>
#include <stdarg.h>

int asprintf(char **restrict ps, const char *restrict fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int rv = vasprintf(ps, fmt, ap);
    va_end(ap);
    return rv;
}
