#include <wchar.h>

wchar_t *wcpncpy(wchar_t *restrict dst, const wchar_t *restrict src, size_t n)
{
    for (; n && *src; n--, *dst++ = *src++);
    for (size_t i = 0; i < n; i++) dst[i] = 0;
    return dst;
}
