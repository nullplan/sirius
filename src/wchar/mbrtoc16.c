#include <uchar.h>
#include <wchar.h>

size_t mbrtoc16(char16_t *restrict pc16, const char *restrict s, size_t n, mbstate_t *restrict ps)
{
    static mbstate_t internal_ps;
    if (!ps) ps = &internal_ps;

    if (!s) {
        pc16 = 0;
        s = "";
        n = 1;
    }

    if ((int)ps->__st > 0) {
        if (pc16) *pc16 = ps->__st;
        ps->__st = 0;
        return -3;
    }

    wchar_t wc;
    size_t l = mbrtowc(&wc, s, n, ps);
    if (l > 4) return l;

    if (wc >= 0x10000) {
        wc -= 0x10000;
        if (pc16) *pc16 = 0xd800 | (wc >> 10);
        ps->__st = 0xdc00 | (wc & 0x3ff);
    } else if (pc16) *pc16 = wc;
    return l;
}
