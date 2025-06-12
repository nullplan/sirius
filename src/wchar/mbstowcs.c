#include <stdlib.h>
#include <wchar.h>

size_t mbstowcs(wchar_t *restrict wcs, const char *restrict s, size_t n)
{
    size_t modified = 0;

    for (;;)
    {
        size_t mbl = mbtowc(wcs, s, -1);
        if (mbl == -1) return -1;
        if (mbl == 0) {
            if (wcs) wcs[modified] = 0;
            return modified;
        }
        if (wcs) wcs++;
        modified++;
        s += mbl;
        if (modified == n) wcs = 0;
    }
}
