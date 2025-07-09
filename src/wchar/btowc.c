#include <wchar.h>
#include <stdlib.h>

wint_t btowc(int c)
{
    if (c < 0x80u) return c;
    if (MB_CUR_MAX == 1 && c < 0x100u) return TO_CODEUNIT(c);
    return WEOF;
}
