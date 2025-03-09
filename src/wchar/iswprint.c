#include <wctype.h>
#include <wchar.h>

static const unsigned char printable[] = {
#include "printable.h"
};

int iswprint(wint_t x)
{
#if 0
    return x - 0x20u < 0x7f - 0x20u || x - 0xa0u < 0x2028 - 0xa0u
        || x - 0x202au < 0xd800 - 0x202au
        || x - 0xe000u < 0xfff9 - 0xe000u
        || (x - 0xfffcu < 0x110000 - 0xfffcu && (x & 0xfffe) != 0xfffe);
#else
    return __is_in_set(x, printable);
#endif
}

static int __iswprint_l(wint_t x, locale_t l)
{
    return iswprint(x);
}
weak_alias(iswprint_l, __iswprint_l);
