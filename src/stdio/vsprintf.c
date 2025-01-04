#include <stdio.h>
#include <stdint.h>
#include "libc.h"

int vsprintf(char *restrict s, const char *restrict fmt, va_list ap)
{
    size_t n = MIN(PTRDIFF_MAX, ~(uintptr_t)s);
    return vsnprintf(s, n, fmt, ap);
}
