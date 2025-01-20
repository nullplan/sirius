#include <wchar.h>
#include <errno.h>
#include <stdlib.h>

size_t wcrtomb(char *restrict s, wchar_t wc, mbstate_t *restrict mbs)
{
    unsigned char *us = (void *)s;
    if (wc < 0x80) {
        *us = wc;
        return 1;
    }

    if (MB_CUR_MAX == 1) {
        if (IS_CODEUNIT(wc)) {
            *us = FROM_CODEUNIT(wc);
            return 1;
        } else {
            errno = EILSEQ;
            return -1;
        }
    }

    /* UTF-8 encodings:
     * 1-byte:  0xxx xxxx: 7 value bits, max CP: 0x7f
     * 2-byte:  110x xxxx 10xx xxxx: 11 value bits, max CP: 0x7ff
     * 3-byte:  1110 xxxx 10xx xxxx 10xx xxxx: 16 value bits, max CP: 0xffff
     * 4-byte:  1111 0xxx 10xx xxxx 10xx xxxx 10xx xxxx: 21 value bits
     */
    if (wc < 0x800) {
        us[0] = 0xC0 | (wc >> 6);
        us[1] = 0x80 | (wc & 0x3f);
        return 2;
    }
    if (wc < 0x10000) {
        us[0] = 0xE0 | (wc >> 12);
        us[1] = 0x80 | ((wc >> 6) & 0x3f);
        us[2] = 0x80 | (wc & 0x3f);
        return 3;
    }
    if (wc < 0x110000) {
        us[0] = 0xF0 | (wc >> 18);
        us[1] = 0x80 | ((wc >> 12) & 0x3f);
        us[2] = 0x80 | ((wc >> 6) & 0x3f);
        us[3] = 0x80 | (wc & 0x3f);
        return 4;
    }
    errno = EILSEQ;
    return -1;
}
