#include <wchar.h>

wchar_t *wcstok(wchar_t *restrict s1, const wchar_t *s2, wchar_t **restrict ptr)
{
    if (!s1) s1 = *ptr;
    else *ptr = s1;
    s1 += wcsspn(s1, s2);
    if (!*s1) return 0;
    wchar_t *rv = s1;
    s1 += wcscspn(s1, s2);
    *s1 = 0;
    *ptr = s1 + 1;
    return rv;
}
