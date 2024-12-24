#include <string.h>
#include <stdint.h>

size_t strlen(const char *s)
{
    size_t rv = 0;
#ifdef __GNUC__
    typedef size_t __attribute__((may_alias)) word_t;
    const size_t ones = -1ul/255;
    const size_t highs = ones << 7;
    while (((uintptr_t)s + rv) & (sizeof (size_t) - 1)) {
        if (!s[rv]) return rv;
        rv++;
    }
    const word_t *ws = (const word_t *)(s + rv);
    for (;;) {
        size_t w = *ws;
        if (~w & (w-ones) & highs)
            break;
        ws++;
    }
    rv = (const char *)ws - s;
#endif
    while (s[rv]) rv++;
    return rv;

}

