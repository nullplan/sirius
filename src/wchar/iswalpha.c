#include <wchar.h>

static const unsigned char alpha[] = {
#include "alpha.h"
};

int iswalpha(wint_t x)
{
    return __is_in_set(x, alpha);
}

static int __iswalpha_l(wint_t x, locale_t l)
{
    return iswalpha(x);
}
weak_alias(iswalpha_l, __iswalpha_l);
