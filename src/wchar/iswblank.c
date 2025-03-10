#include <wchar.h>

int iswblank(wint_t x)
{
    return x == ' ' || x == '\t';
}

static int __iswblank_l(wint_t x, locale_t l)
{
    return iswblank(x);
}
weak_alias(iswblank_l, __iswblank_l);
