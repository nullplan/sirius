#include <uchar.h>
#include <wchar.h>

size_t mbrtoc32(char32_t *restrict pc32, const char *restrict s, size_t n, mbstate_t *restrict ps)
{
    static mbstate_t intps;
    if (!ps) ps = &intps;

    return mbrtowc((wchar_t *)pc32, s, n, ps);
}
