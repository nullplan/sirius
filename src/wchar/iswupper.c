#include <wctype.h>

int iswupper(wint_t x)
{
    return towlower(x) != x;
}
static int __iswupper_l(wint_t x, locale_t l)
{
    return iswupper(x);
}
weak_alias(iswupper_l, __iswupper_l);
