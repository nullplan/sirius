#include <wchar.h>

int iswcntrl(wint_t x)
{
    return (unsigned)x < 0x20 || x - 0x7fu < 0xa0 - 0x7fu || x - 0x2028u < 2u;
}

static int __iswcntrl_l(wint_t x, locale_t l)
{
    return iswcntrl(x);
}
weak_alias(iswcntrl_l, __iswcntrl_l);
