#include <stdlib.h>
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <errno.h>

static uintmax_t strtoint(const char *str, char **endp, int base, uintmax_t limit)
{
    if (base < 0 || base == 1 || base > 36) {
        errno = EINVAL;
        return 0;
    }
    FILE f;
    sh_fromstr(&f, str);
    shlim(&f, 0);

    uintmax_t ret = __intscan(&f, base, limit);
    if (endp)
        *endp = (char *)str + shcnt(&f);
    return ret;
}


long strtol(const char *restrict str, char **restrict endp, int base)
{
    return strtoint(str, endp, base, LONG_MIN);
}

long long strtoll(const char *restrict str, char **restrict endp, int base)
{
    return strtoint(str, endp, base, LLONG_MIN);
}

unsigned long strtoul(const char *restrict str, char **restrict endp, int base)
{
    return strtoint(str, endp, base, ULONG_MAX);
}

unsigned long long strtoull(const char *restrict str, char **restrict endp, int base)
{
    return strtoint(str, endp, base, ULLONG_MAX);
}

intmax_t strtoimax(const char *restrict str, char **restrict endp, int base)
{
    return strtoint(str, endp, base, INTMAX_MIN);
}

uintmax_t strtoumax(const char *restrict str, char **restrict endp, int base)
{
    return strtoint(str, endp, base, UINTMAX_MAX);
}
