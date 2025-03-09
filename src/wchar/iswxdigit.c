#include <wctype.h>

int iswxdigit(wint_t x)
{
    return x - '0' < 10u || (x|32) - 'a' < 6u;
}
static int __iswxdigit_l(wint_t x, locale_t l)
{
    return iswxdigit(x);
}
weak_alias(iswxdigit_l, __iswxdigit_l);
