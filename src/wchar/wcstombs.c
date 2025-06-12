#include <stdlib.h>
#include <string.h>

size_t wcstombs(char *restrict s, const wchar_t *restrict wcs, size_t n)
{
    char buf[MB_LEN_MAX];
    size_t outlen = 0;
    for (;;)
    {
        int mbl = wctomb(buf, *wcs);
        if (mbl == -1) return -1;
        if (s) {
            if (outlen + mbl >= n) return outlen;
            memcpy(s + outlen, buf, mbl);
        }
        if (!wcs) return outlen;
        outlen += mbl;
        wcs++;
    }
}
