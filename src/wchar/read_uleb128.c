#include <wchar.h>

extern hidden unsigned __read_uleb128(const unsigned char **pp)
{
    const unsigned char *p = *pp;
    unsigned rv = 0;
    int k = 0;
    do {
        rv |= (*p & 0x7f) << k;
        k += 7;
    } while (*p++ & 0x80);
    *pp = p;
    return rv;
}
