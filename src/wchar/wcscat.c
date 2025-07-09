#include <wchar.h>

wchar_t *wcscat(wchar_t *restrict dst, const wchar_t *restrict src)
{
    wcscpy(dst + wcslen(dst), src);
    return dst;
}
