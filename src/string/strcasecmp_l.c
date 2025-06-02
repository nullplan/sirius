#include <strings.h>
#include <locale.h>
int strcasecmp_l(const char *a, const char *b, locale_t loc)
{
    locale_t old = uselocale(loc);
    int rv = strcasecmp(a, b);
    uselocale(old);
    return rv;
}
