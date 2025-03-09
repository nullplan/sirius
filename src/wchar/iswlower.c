#include <wctype.h>

int iswlower(wint_t x)
{
    return towupper(x) != x;
}

static int __iswlower_l(wint_t x, locale_t l)
{
    return iswlower(x);
}
weak_alias(iswlower_l, __iswlower_l);
