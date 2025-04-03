#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include "libc.h"

locale_t duplocale(locale_t loc)
{
    if (loc == LC_GLOBAL_LOCALE) loc = &__global_locale;
    locale_t newloc = malloc(sizeof (struct __localedef));
    if (newloc) memcpy((void *)newloc, loc, sizeof (struct __localedef));
    return newloc;
}
