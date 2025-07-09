#include <wchar.h>

wchar_t *(wcschr)(const wchar_t *wcs, wchar_t x)
{
    size_t i = __wcsidx(wcs, x);
    if (!wcs[i]) return 0;
    return (wchar_t *)wcs + i;
}
