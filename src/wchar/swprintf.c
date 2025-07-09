#include <wchar.h>
#include <stdarg.h>

int swprintf(wchar_t *restrict s, size_t n, const wchar_t *restrict fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int rv = vswprintf(s, n, fmt, ap);
    va_end(ap);
    return rv;
}
