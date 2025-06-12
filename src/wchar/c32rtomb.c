#include <uchar.h>
#include <wchar.h>

size_t c32rtomb(char *restrict s, char32_t c32, mbstate_t *restrict ps)
{
    static mbstate_t intps;
    if (!ps) ps = &intps;

    return wcrtomb(s, c32, ps);
}
