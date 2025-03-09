#include <wctype.h>

int iswalnum(wint_t x)
{
    return iswalpha(x) || iswdigit(x);
}

static int __iswalnum_l(wint_t x, locale_t l)
{
    return iswalnum(x);
}
weak_alias(iswalnum_l, __iswalnum_l);
