#include <wctype.h>
#include <string.h>

int iswctype(wint_t c, wctype_t t)
{
    static int (*const funcs[])(wint_t) = {
        iswalnum, iswalpha, iswblank, iswcntrl, iswdigit, iswgraph,
        iswlower, iswprint, iswpunct, iswspace, iswupper, iswxdigit,
    };
    if (t - 1u >= sizeof funcs / sizeof *funcs)
        return 0;
    return funcs[t-1](c);
}
static int __iswctype_l(wint_t x, wctype_t t, locale_t l)
{
    return iswctype(x, t);
}
weak_alias(iswctype_l, __iswctype_l);

wctype_t wctype(const char *t)
{
    static const char names[] =
        "alnum\0" "alpha\0" "blank\0" "cntrl\0" "digit\0" "graph\0"
        "lower\0" "print\0" "punct\0" "space\0" "upper\0" "xdigit";
    wctype_t i = 1;
    for (const char *p = names; *p; p += 6, i++)
        if (!strcmp(t, p))
            return i;
    return 0;
}
