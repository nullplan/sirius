#include <wchar.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

size_t mbrtowc(wchar_t *restrict wc, const char *restrict s, size_t len,
        mbstate_t *restrict state)
{
    static mbstate_t internal_state;
    if (!state) state = &internal_state;
    unsigned v = state->__st; state->__st = 0;
    size_t n = 0;

    if (!s) {
        if (v) goto ilseq;
        return 0;
    }
    
    if (!len) return -2;
    const unsigned char *us = (void *)s;
    if (!v) {
        if (*us < 0x80 || MB_CUR_MAX == 1) {
            if (wc) *wc = TO_CODEUNIT(*us);
            return !!*us;
        }
        if (*us - UTF_START + 0u > UTF_END - UTF_START + 0u) goto ilseq;
        v = __utftab[*us++ - UTF_START];
        n++;
    }

    while (n < len && (v & (1ul << 31))) {
        unsigned base = v >> 28;
        unsigned lim = (v >> 26) & 3;
        if ((*us >> 4) - base > lim) goto ilseq;
        v <<= 6;
        v |= *us++ & 0x3F;
        n++;
    }

    if (v & (1ul << 31)) {
        state->__st = v;
        return -2;
    }
    if (wc) *wc = v;
    return n;
ilseq:
    errno = EILSEQ;
    return -1;
}
