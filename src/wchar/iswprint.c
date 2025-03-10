#include <wctype.h>
#include <wchar.h>

static const unsigned char printable[] = {
#include "printable.h"
};

int iswprint(wint_t x)
{
    return __is_in_set(x, printable, sizeof printable);
}

static int __iswprint_l(wint_t x, locale_t l)
{
    return iswprint(x);
}
weak_alias(iswprint_l, __iswprint_l);
