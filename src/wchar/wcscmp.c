#include <wchar.h>

int wcscmp(const wchar_t *a, const wchar_t *b)
{
    while (*a && *a == *b) a++, b++;
    return *a < *b? -1 : *a > *b? 1 : 0;
}
