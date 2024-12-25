#include <string.h>

size_t strlcpy(char *restrict dst, const char *restrict src, size_t n)
{
    if (!n) return strlen(src);
    size_t i;
    for (i = 0; i < n - 1 && src[i]; i++)
        dst[i] = src[i];
    dst[i] = 0;
    return i + strlen(src + i);
}
