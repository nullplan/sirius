#include <string.h>

hidden char *__stpncpy(char *restrict dst, const char *restrict src, size_t n)
{
    while (n && *src) {
        *dst++ = *src++;
        n--;
    }
    memset(dst, 0, n);
    return dst;
}
weak_alias(stpncpy, __stpncpy);
