#include <wchar.h>
#include <stdio.h>
#include <stdarg.h>

int wprintf(const wchar_t *restrict fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int rv = vfwprintf(stdout, fmt, ap);
    va_end(ap);
    return rv;
}
