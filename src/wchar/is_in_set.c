#include <wchar.h>

hidden int __is_in_set(wint_t x, const unsigned char *set, size_t len)
{
    wint_t num = 0;
    int res = 0;
    for (const unsigned char *p = set; p < set + len;)
    {
        unsigned block = __read_uleb128(&p);
        if (x - num < block)
            return res;
        num += block;
        res = 1 - res;
    }
    return 0;
}
