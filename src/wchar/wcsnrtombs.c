#include <wchar.h>
#include <stdlib.h>
#include <string.h>

size_t wcsnrtombs(char *restrict dst, const wchar_t **restrict psrc, size_t nwc, size_t len, mbstate_t *restrict ps)
{
    static mbstate_t internal;
    if (!ps) ps = &internal;
    const wchar_t *src = *psrc;
    size_t mbslen = 0;

    if (!dst) {
        while (nwc && *src) {
            size_t mblen = wcrtomb(0, *src, ps);
            if (mblen == -1) return -1;
            mbslen += mblen;
            src++;
            nwc--;
        }
        return mbslen;
    }

    if (!len) return 0;

    while (nwc && *src)
    {
        size_t mblen;
        if (len >= MB_CUR_MAX)
            mblen = wcrtomb(dst, *src, ps);
        else {
            char buf[MB_LEN_MAX];
            mblen = wcrtomb(buf, *src, ps);
            if (mblen <= len)
                memcpy(dst, buf, mblen);
        }

        if (mblen == -1) {
            *psrc = src;
            return -1;
        }
        if (mblen > len) {
            *psrc = src;
            return mbslen;
        }
        src++;
        nwc--;
        mbslen += mblen;
        dst += mblen;
        len -= mblen;
        if (!len) {
            *psrc = src;
            return mbslen;
        }
    }
    *dst = 0;
    *psrc = 0;
    return mbslen;
}
