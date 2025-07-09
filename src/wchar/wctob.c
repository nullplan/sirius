#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>

int wctob(wint_t wc)
{
    if (wc < 0x80u) return wc;
    if (MB_CUR_MAX == 1 && IS_CODEUNIT(wc)) return FROM_CODEUNIT(wc);
    return EOF;
}
