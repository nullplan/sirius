#include <wchar.h>
#include "libc.h"

size_t wcslcpy(wchar_t *restrict dst, const wchar_t *restrict src, size_t n)
{
    size_t srclen = wcslen(src);
    if (n) {
        size_t copylen = MIN(srclen, n - 1);
        wmemcpy(dst, src, copylen + 1);
    }
    return srclen;
}
