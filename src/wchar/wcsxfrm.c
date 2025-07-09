#include <wchar.h>
#include <string.h>

size_t wcsxfrm(wchar_t *restrict dst, const wchar_t *restrict src, size_t n)
{
    size_t l = wcslen(src);
    if (l < n) memcpy(dst, src, l * sizeof (wchar_t));
    return l + 1;
}

static size_t __wcsxfrm_l(wchar_t *restrict dst, const wchar_t *restrict src, size_t n, locale_t l)
{
    return wcsxfrm(dst, src, n);
}
weak_alias(wcsxfrm_l, __wcsxfrm_l);
