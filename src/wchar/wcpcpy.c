#include <wchar.h>

wchar_t *wcpcpy(wchar_t *restrict dst, const wchar_t *restrict src)
{
    while ((*dst++ = *src++));
    return dst - 1;
}
