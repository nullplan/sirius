#include <string.h>
#include <errno.h>

static const char *const errtab[] = {
#define E(code, str) [code] = str,
#include "strerror.h"
};

char *strerror(int e)
{
    if (e >= sizeof errtab/sizeof *errtab || !errtab[e]) return "Unknown error";
    return (char *)errtab[e];
}

static char *__strerror_l(int e, locale_t l)
{
    return strerror(e);
}
weak_alias(strerror_l, __strerror_l);
