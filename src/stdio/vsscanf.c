#include <stdio.h>

int vsscanf(const char *restrict s, const char *restrict fmt, va_list ap)
{
    FILE f;
    sh_fromstr(&f, s);
    f.lock = -1;
    return vfscanf(&f, fmt, ap);
}
