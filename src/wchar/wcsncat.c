#include <wchar.h>

wchar_t *wcsncat(wchar_t *restrict dst, const wchar_t *restrict src, size_t n)
{
    wchar_t *mid = dst + wcslen(dst);
    size_t i;
    for (i = 0; i < n && src[i]; i++)
        mid[i] = src[i];
    mid[i] = 0;
    return dst;
}
