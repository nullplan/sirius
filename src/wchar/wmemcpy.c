#include <wchar.h>
#include <string.h>

wchar_t *wmemcpy(wchar_t *restrict dst, const wchar_t *restrict src, size_t n)
{
    return memcpy(dst, src, n * sizeof (wchar_t));
}
