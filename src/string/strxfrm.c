#include <string.h>
size_t strxfrm(char *restrict dst, const char *restrict src, size_t n)
{
    size_t l = strlen(src);
    if (l < n) memcpy(dst, src, l + 1);
    return l;
}

static size_t __strxfrm_l(char *restrict dst, const char *restrict src, size_t n, locale_t l)
{
    return strxfrm(dst, src, n);
}
weak_alias(strxfrm_l, __strxfrm_l);
