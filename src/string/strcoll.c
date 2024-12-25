#include <string.h>

int strcoll(const char *a, const char *b)
{
    return strcmp(a, b);
}

static int __strcoll_l(const char *a, const char *b, locale_t l)
{
    return strcoll(a, b);
}
weak_alias(strcoll_l, __strcoll_l);
