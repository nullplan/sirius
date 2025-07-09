#include <wchar.h>
#include <stdarg.h>
int fwprintf(FILE *restrict f, const wchar_t *restrict fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int rv = vfwprintf(f, fmt, ap);
    va_end(ap);
    return rv;
}
