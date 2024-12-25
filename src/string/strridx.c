#include <string.h>
#include <stdint.h>

hidden size_t __strridx(const char *s, int c)
{
    size_t rv = 0;
    size_t i = 0;
    c &= 0xff;
#ifdef __GNUC__
    typedef size_t __attribute__((may_alias)) word_t;
    while ((uintptr_t)(s + i) & (sizeof (size_t) - 1)) {
        if ((unsigned char)s[i] == c)
            rv = i;
        if (s[i] == 0) return rv;
        i++;
    }
    const word_t *ws = (const void *)(s + i);
    const size_t ones = -1ul / 255;
    const size_t highs = ones << 7;
    const size_t wmask = c * ones;
    for (;;) {
        size_t w = *ws;
        if (~w & (w-ones) & highs) break;
        w ^= wmask;
        if (~w & (w-ones) & highs)
        {
            size_t off = (const char *)ws - s;
            for (size_t i = 0; i < sizeof (size_t); i++)
                if (s[off+i] == c)
                    rv = off+i;
        }
        ws++;
    }
    i = (const char *)ws - s;
#endif
    for (; s[i]; i++)
        if (s[i] == c)
            rv = i;
    return rv;
}
