#include <string.h>

size_t strlcat(char *restrict dst, const char *restrict src, size_t n)
{
    if (!n) return strlen(src);
    size_t dstlen = strnlen(dst, n);
    return dstlen + strlcpy(dst + dstlen, src, n - dstlen);
}
