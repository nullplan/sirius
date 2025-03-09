#include <wctype.h>

int iswpunct(wint_t x)
{
    return iswgraph(x) && !iswalnum(x);
}

static int __iswpunct_l(wint_t x, locale_t l)
{
    return iswpunct(x);
}
weak_alias(iswpunct_l, __iswpunct_l);
