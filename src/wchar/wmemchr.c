#include <wchar.h>

wchar_t *(wmemchr)(const wchar_t *s, wchar_t c, size_t n)
{
    size_t i;
    for (i = 0; i < n && s[i] != c; i++);
    if (i == n) return 0;
    return (wchar_t *)s + i;
}
