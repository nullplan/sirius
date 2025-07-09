#include <wchar.h>

int wcsncmp(const wchar_t *a, const wchar_t *b, size_t n)
{
    if (!n) return 0;
    while (--n && *a && *a == *b) a++, b++;
    return *a < *b? -1 : *a > *b? 1 : 0;
}
