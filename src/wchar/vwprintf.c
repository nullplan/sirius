#include <wchar.h>
#include <stdio.h>

int vwprintf(const wchar_t *restrict fmt, va_list ap)
{
    return vfwprintf(stdout, fmt, ap);
}
