#include <wchar.h>
#include "libc.h"

size_t wcslcat(wchar_t *restrict dst, const wchar_t *restrict src, size_t n)
{
    size_t dstlen = wcslen(dst);
    size_t srclen = wcslen(src);
    if (dstlen < n) {
        size_t copylen = MIN(srclen, n - dstlen - 1);
        wmemcpy(dst + dstlen, src, copylen);
        dst[dstlen + copylen] = 0;
    }
    return dstlen + srclen;
}
