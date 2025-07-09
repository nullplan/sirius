#include <wchar.h>

size_t mbsnrtowcs(wchar_t *restrict dst, const char **restrict psrc, size_t nmc, size_t len, mbstate_t *restrict ps)
{
    static mbstate_t internal;
    if (!ps) ps = &internal;

    const char *src = *psrc;
    size_t converted = 0;
    if (!dst) {
        for (;;) {
            size_t mblen = mbrtowc(0, src, nmc, ps);
            if (mblen == -1) return -1;
            if (mblen == 0 || mblen == -2) return converted;
            converted++;
            src += mblen;
            nmc -= mblen;
        }
    }

    if (!len) return 0;

    for (;;) {
        size_t mblen = mbrtowc(dst, src, nmc, ps);
        if (mblen == -1) {
            *psrc = src;
            return -1;
        }
        if (mblen == -2) {
            *psrc = src + nmc;
            return converted;
        }
        if (mblen == 0) {
            *psrc = 0;
            return converted;
        }
        converted++;
        src += mblen;
        dst++;
        if (!--len) {
            if (dst) *psrc = src;
            return converted;
        }
    }
}
