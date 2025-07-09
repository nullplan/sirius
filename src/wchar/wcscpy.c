#include <wchar.h>

wchar_t *wcscpy(wchar_t *restrict dst, const wchar_t *restrict src)
{
    for (size_t i = 0; dst[i] = src[i]; i++);
    return dst;
}
