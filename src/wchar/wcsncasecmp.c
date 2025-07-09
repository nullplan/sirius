#include <wchar.h>
#include <wctype.h>

int wcsncasecmp(const wchar_t *a, const wchar_t *b, size_t n)
{
    while (n && *a && towlower(*a) == towlower(*b)) a++, b++, n--;
    if (!n) return 0;
    wchar_t x = towlower(*a), y = towlower(*b);
    return x < y? -1 : x > y? 1 : 0;
}

int wcsncasecmp_l(const wchar_t *a, const wchar_t *b, size_t n, locale_t l)
{
    return wcsncasecmp(a, b, n);
}
