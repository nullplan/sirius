#include <wchar.h>
#include <string.h>

wchar_t *wmemmove(wchar_t *dst, const wchar_t *src, size_t n)
{
    return memmove(dst, src, n * sizeof (wchar_t));
}
