#include <uchar.h>
#include <wchar.h>
#include <stdlib.h>
#include <errno.h>

size_t c16rtomb(char *restrict s, char16_t c16, mbstate_t *restrict ps)
{
    static mbstate_t internal_ps;
    if (!ps) ps = &internal_ps;

    char buf[4];
    if (!s) {
        s = buf;
        c16 = 0;
    }
    
    wchar_t wc;
    unsigned st = ps->__st;
    ps->__st = 0;
    if (!st) {
        if (c16 - 0xd800u < 0x400u) {
            ps->__st = c16 - 0xd800 + 0x40;
            return 0;
        }
        wc = c16;
    } else {
        if (c16 - 0xdc00u >= 0x400u) {
            errno = EILSEQ;
            return -1;
        }
        wc = st << 10 | (c16 - 0xdc00);
    }

    return wctomb(s, wc);
}
