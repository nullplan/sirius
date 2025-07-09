#include <wchar.h>

wchar_t *wcspbrk(const wchar_t *wcs, const wchar_t *stop)
{
    size_t l = wcscspn(wcs, stop);
    if (!wcs[l]) return 0;
    return (wchar_t *)wcs + l;
}
