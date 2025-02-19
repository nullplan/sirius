#include <stdlib.h>
#include <stdio.h>
#include <float.h>

static long double strtox(const char *restrict s, char **restrict endp, int bits, int emin)
{
    FILE f;
    sh_fromstr(&f, s);
    shlim(&f, 0);
    long double r = __floatscan(&f, 1, bits, emin);
    if (endp)
        *endp = (char *)s + shcnt(&f);
    return r;
}

float strtof(const char *restrict s, char **restrict endp)
{
    return (float)strtox(s, endp, FLT_MANT_DIG, FLT_MIN_EXP);
}

double strtod(const char *restrict s, char **restrict endp)
{
    return (double)strtox(s, endp, DBL_MANT_DIG, DBL_MIN_EXP);
}

long double strtold(const char *restrict s, char **restrict endp)
{
    return strtox(s, endp, LDBL_MANT_DIG, LDBL_MIN_EXP);
}
