#include <wchar.h>

static const unsigned char __casemap[] = {
#include "casemap.h"
};

static int read_sleb128(const unsigned char **pp)
{
    const unsigned char *p = *pp;
    unsigned rv = 0;
    int k = 0;
    do {
        rv |= (*p & 0x7f) << k;
        k += 7;
    } while (*p++ & 0x80);
    if (rv & (1u << (k - 1)))
        rv |= -1u << k;
    *pp = p;
    return rv;
}

static wint_t casemap(wint_t x, int lower)
{
    const unsigned char *p = __casemap;
    wint_t u = 0;
    while (p < __casemap + sizeof __casemap) {
        unsigned off = __read_uleb128(&p);
        unsigned len = __read_uleb128(&p);
        int diff = read_sleb128(&p);
        int stride = off == 1? 2 : 1;
        u += off;
        wint_t upper = lower? x - diff : x;
        if (upper - u < len && !((upper - u) & (stride - 1)))
            return lower? upper : upper + diff;
        u += len;
    }
    return x;
}

wint_t towlower(wint_t x)
{
    return casemap(x, 0);
}
static wint_t __towlower_l(wint_t x, locale_t l)
{
    return towlower(x);
}
weak_alias(towlower_l, __towlower_l);

wint_t towupper(wint_t x)
{
    return casemap(x, 1);
}
static wint_t __towupper_l(wint_t x, locale_t l)
{
    return towupper(x);
}
weak_alias(towupper_l, __towupper_l);
