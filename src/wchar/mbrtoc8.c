#include <uchar.h>
#include <wchar.h>
#include <stdlib.h>
size_t mbrtoc8(char8_t *restrict pc8, const char *restrict s, size_t n, mbstate_t *restrict ps)
{
    static mbstate_t internal_ps;
    if (!ps) ps = &internal_ps;
    if (!s) {
        pc8 = 0;
        s = "";
        n = 1;
    }

    if (ps->__st - 1u < 0x7fffffffU) {
        if (pc8) *pc8 = 0x80 | (ps->__st & 0x3f);
        ps->__st >>= 6;
        return -3;
    }

    wchar_t wc;
    size_t mbl = mbrtowc(&wc, s, n, ps);
    size_t enclen = mbl;
    if (mbl == 1 && MB_CUR_MAX == 1 && wc > 0x80) enclen = 2;
    switch (enclen)
    {
        case 0:
        case 1:
            if (pc8) *pc8 = wc;
            break;

        case 2:
            if (pc8) *pc8 = 0xc0 | (wc >> 6);
            ps->__st = wc & 0x3f;
            break;

        case 3:
            if (pc8) *pc8 = 0xe0 | (wc >> 12);
            ps->__st = ((wc >> 6) & 0x3f) | ((wc & 0x3f) << 6);
            break;

        case 4:
            if (pc8) *pc8 = 0xf0 | (wc >> 18);
            ps->__st = ((wc >> 12) & 0x3f) | (wc & 0xfc) | ((wc & 0x3f) << 6);
            break;
    }
    return mbl;
}
