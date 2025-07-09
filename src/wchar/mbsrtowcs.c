#include <wchar.h>

size_t mbsrtowcs(wchar_t *restrict dst, const char **restrict psrc, size_t len, mbstate_t *restrict ps)
{
    static mbstate_t internal;
    if (!ps) ps = &internal;

    const char *src = *psrc;
    size_t converted = 0;
    if (!dst) {
        for (;;) {
            size_t mblen = mbrtowc(0, src, -1, ps);
            if (mblen == -1) return -1;
            if (mblen == 0) return converted;
            converted++;
            src += mblen;
        }
    }

    if (!len) return 0;

    for (;;) {
        size_t mblen = mbrtowc(dst, src, -1, ps);
        if (mblen == -1) {
            *psrc = src;
            return -1;
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
