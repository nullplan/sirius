#include <strings.h>
#include <locale.h>

int strncasecmp_l(const char *a, const char *b, size_t n, locale_t loc)
{
    locale_t old = uselocale(loc);
    int rv = strncasecmp(a, b, n);
    uselocale(old);
    return rv;
}
