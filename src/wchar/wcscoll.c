#include <wchar.h>

int wcscoll(const wchar_t *a, const wchar_t *b)
{
    return wcscmp(a, b);
}

static int __wcscoll_l(const wchar_t *a, const wchar_t *b, locale_t l)
{
    return wcscoll(a, b);
}
weak_alias(wcscoll_l, __wcscoll_l);
