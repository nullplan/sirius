#include <wchar.h>

wchar_t *wcsncpy(wchar_t *restrict dst, const wchar_t *restrict src, size_t n)
{
    size_t i;
    for (i = 0; i < n && (dst[i] = src[i]); i++);
    for (; i < n; i++) dst[i] = 0;
    return dst;
}
