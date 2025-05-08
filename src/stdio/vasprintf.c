#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int vasprintf(char **restrict ps, const char *restrict fmt, va_list ap)
{
    va_list ap2;
    va_copy(ap2, ap);
    int len = vsnprintf(0, 0, fmt, ap2);
    va_end(ap2);
    if (len < 0) return len;
    *ps = malloc(len + 1);
    if (!*ps) return -1;
    return vsnprintf(*ps, len + 1, fmt, ap);
}
