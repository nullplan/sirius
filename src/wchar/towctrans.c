#include <wctype.h>
#include <string.h>

wint_t towctrans(wint_t x, wctrans_t t)
{
    if (t == 1) return towlower(x);
    if (t == 2) return towupper(x);
    return x;
}
static wint_t __towctrans_l(wint_t x, wctrans_t t, locale_t l)
{
    return towctrans(x, t);
}
weak_alias(towctrans_l, __towctrans_l);

wctrans_t wctrans(const char *t)
{
    if (!strcmp(t, "tolower")) return 1;
    if (!strcmp(t, "toupper")) return 2;
    return 0;
}
