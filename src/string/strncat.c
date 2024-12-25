#include <string.h>

char *strncat(char *restrict dst, const char *restrict src, size_t n)
{
    char *r = dst;
    while (*dst) dst++;
    while (n && *src) {
        *dst++ = *src++;
        n--;
    }
    *dst = 0;
    return r;
}
