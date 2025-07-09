#include <wchar.h>

hidden size_t __wcsidx(const wchar_t *wcs, wchar_t x)
{
    size_t i;
    for (i = 0; wcs[i] && wcs[i] != x; i++);
    return i;
}
