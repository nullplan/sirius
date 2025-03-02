#include <stdlib.h>
#include <wchar.h>
#include <errno.h>

int mbtowc(wchar_t *restrict wc, const char *restrict s, size_t n)
{
    if (!s) return 0;
    if (!n) goto ilseq;

    unsigned char uc = *s;
    if (uc < 0x80 || MB_CUR_MAX == 1) {
        if (wc) *wc = TO_CODEUNIT(uc);
        return !!uc;
    }

    if (uc - UTF_START + 0u > UTF_END - UTF_START + 0u) goto ilseq;
    unsigned state = __utftab[uc - UTF_START];
    const unsigned char *us = (void *)(s + 1);

    /* simple test: if n is not big enough for the biggest encoding
     * and even presuming we can consume n bytes we cannot shift away the top bit
     * then the character is incomplete.
     */
    int rv = 1;
    if (n < 4 && ((state << (6 * n)) & 0x80000000)) goto ilseq;
    while (state & 0x80000000)
    {
        unsigned base = state >> 28;
        unsigned range = (state >> 26) & 3;
        uc = *us++;
        if ((uc >> 4) - base > range) goto ilseq;
        state = (state << 6) | (uc & 0x3f);
        rv++;
    }

    if (wc) *wc = state;
    return rv;

ilseq:
    errno = EILSEQ;
    return -1;
}
