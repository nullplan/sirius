#include <stdlib.h>
#include <wchar.h>
#include <errno.h>

int wctomb(char *s, wchar_t wc)
{
    if (!s) return 0;
    return wcrtomb(s, wc, 0);
}
