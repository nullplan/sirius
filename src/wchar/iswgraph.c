#include <wctype.h>

int iswgraph(wint_t x)
{
    return iswprint(x) && !iswspace(x);
}

static int __iswgraph_l(wint_t x, locale_t l)
{
    return iswgraph(x);
}
weak_alias(iswgraph_l, __iswgraph_l);
