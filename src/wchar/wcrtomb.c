#include <wchar.h>
#include <errno.h>
#include <stdlib.h>

size_t wcrtomb(char *restrict s, wchar_t wc, mbstate_t *restrict mbs)
{
    char buf[4];
    if (!s) {
        s = buf;
        wc = 0;
    }
    if (wc < 0x80u || (MB_CUR_MAX == 1 && IS_CODEUNIT(wc))) {
        *s = wc;
        return 1;
    }

    if (MB_CUR_MAX == 1) goto ilseq;

    if (wc < 0x800u) {
        s[0] = 0xc0 | (wc >> 6);
        s[1] = 0x80 | (wc & 0x3f);
        return 2;
    }
    if (wc < 0x10000u) {
        s[0] = 0xe0 | (wc >> 12);
        s[1] = 0x80 | ((wc >> 6) & 0x3f);
        s[2] = 0x80 | (wc & 0x3f);
        return 3;
    }
    if (wc < 0x110000u) {
        s[0] = 0xf0 | (wc >> 18);
        s[1] = 0x80 | ((wc >> 12) & 0x3f);
        s[2] = 0x80 | ((wc >> 6) & 0x3f);
        s[3] = 0x80 | (wc & 0x3f);
        return 4;
    }

ilseq:
    errno = EILSEQ;
    return -1;
}
