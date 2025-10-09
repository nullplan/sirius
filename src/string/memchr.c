#include <string.h>
#include <stdint.h>

void *(memchr)(const void *s, int c, size_t n)
{
    const unsigned char *us = s;
    c &= 0xff;
#ifdef __GNUC__
    typedef size_t __attribute__((may_alias)) word_t;
    if (n >= 2 * sizeof (size_t)) {
        size_t max = (-(uintptr_t)us) & (sizeof (size_t) - 1);
        for (size_t i = 0; i < max; i++)
            if (us[i] == c)
                return (void *)(us + i);
        n -= max;
        us += max;
        const word_t *ws = (const void *)us;
        const size_t ones = -1ul/255;
        const size_t highs = ones << 7;
        const size_t mask = c * ones;
        while (n >= sizeof (size_t)) {
            size_t w = *ws ^ mask;
            if (~w & (w-ones) & highs) break;
            ws++;
            n -= sizeof (size_t);
        }
        us = (const void *)ws;
    }
#endif
    while (n--)
        if (*us++ == c)
            return (void *)(us - 1);
    return 0;
}
