#include <uchar.h>
#include <wchar.h>
#include <stdlib.h>
#include <errno.h>

size_t c8rtomb(char *restrict s, char8_t c8, mbstate_t *restrict ps)
{
    static mbstate_t internal_ps;
    char buf[4];

    if (!ps) ps = &internal_ps;
    if (!s) {
        s = buf;
        c8 = 0;
    }
    unsigned st = ps->__st;
    ps->__st = 0;

    if (!st) {
        if (c8 < 0x80) {
            *s = c8;
            return 1;
        }
        if (c8 - UTF_START + 0u > UTF_END - UTF_START + 0u) {
            errno = EILSEQ;
            return -1;
        }
        ps->__st = __utftab[c8 - UTF_START];
        return 0;
    }

    unsigned base = st >> 28;
    unsigned lim = (st >> 26) & 3;
    if ((c8 >> 4) - base > lim) {
        errno = EILSEQ;
        return -1;
    }

    st = (st << 6) | (c8 & 0x3f);
    if (st & 0x80000000) {
        ps->__st = st;
        return 0;
    }

    return wctomb(s, st);
}
