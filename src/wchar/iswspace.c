#include <wchar.h>

int iswspace(wint_t x)
{
    /* union of the musl and glibc sets */
    return x - 9u < 5u || x == ' ' || x == 0x85 || x == 0x1680
        || (x - 0x2000u < 11u && x != 0x2007) || x == 0x2028 || x == 0x2029
        || x == 0x205f || x == 0x3000;
}

static int __iswspace_l(wint_t x, locale_t l)
{
    return iswspace(x);
}
weak_alias(iswspace_l, __iswspace_l);
