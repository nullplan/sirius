#include <string.h>
#include <stdint.h>

hidden size_t __stridx(const char *s, int c)
{
    size_t rv = 0;
#ifdef __GNUC__
    typedef size_t __attribute__((may_alias)) word_t;
    while ((uintptr_t)(s + rv) & (sizeof (size_t) - 1)) {
        if ((unsigned char)s[rv] == c || !s[rv])
            return rv;
        rv++;
    }
    const word_t *ws = (const void *)(s + rv);
    const size_t ones = -1ul / 255;
    const size_t highs = ones << 7;
    const size_t wmask = c * ones;
    for (;;) {
        size_t w = *ws;
        if (~w & (w-ones) & highs) break;
        w ^= wmask;
        if (~w & (w-ones) & highs) break;
        ws++;
    }
    rv = (const char *)ws - s;
#endif
    for (; s[rv] && (unsigned char)s[rv] != c; rv++);
    return rv;
}
