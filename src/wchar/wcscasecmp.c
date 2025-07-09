#include <wchar.h>
#include <wctype.h>

int wcscasecmp(const wchar_t *a, const wchar_t *b)
{
    while (*a && towlower(*a) == towlower(*b)) a++, b++;
    wchar_t x = towlower(*a), y = towlower(*b);
    return x < y? -1 : x > y? 1 : 0;
}

int wcscasecmp_l(const wchar_t *a, const wchar_t *b, locale_t l)
{
    return wcscasecmp(a, b);
}
