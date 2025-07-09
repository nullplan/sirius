#include <wchar.h>

wchar_t *wcsrchr(const wchar_t *s, wchar_t x)
{
    wchar_t *rv = 0;
    for (size_t i = 0; s[i]; i++)
        if (s[i] == x)
            rv = (wchar_t *)s + i;
    return rv;
}
