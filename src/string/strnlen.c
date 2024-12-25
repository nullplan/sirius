#include <string.h>

size_t strnlen(const char *str, size_t n)
{
    const char *p = memchr(str, 0, n);
    if (!p) return n;
    return p - str;
}
