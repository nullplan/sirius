#include <string.h>

void *memccpy(void *restrict dst, const void *restrict src, int c, size_t n)
{
    const void *p = memchr(src, c, n);
    if (p) {
        n = (char *)p - (char *)src + 1;
        memcpy(dst, src, n);
        return (char *)dst + n;
    } else {
        memcpy(dst, src, n);
        return 0;
    }
}
