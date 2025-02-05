#include <stdio.h>
#include <stdlib.h>

int vasprintf(char **restrict ps, const char *restrict fmt, va_list ap)
{
    int len = vsnprintf(0, 0, fmt, ap);
    if (len < 0) return len;
    *ps = malloc(len + 1);
    if (!*ps) return -1;
    return vsnprintf(*ps, len + 1, fmt, ap);
}
