#include <netdb.h>
#include <string.h>

hidden int __dn_expand(const unsigned char *p, const unsigned char *packet, size_t plen, char *buf, size_t buflen)
{
    int first = 1;
    const unsigned char *end = packet + plen;
    for (size_t i = 0; *p && i < 2 * plen; i++)
    {
        if (!*p) break;
        if (*p < 64) {
            if (buflen < *p + 1 || end - p < *p + 1)
                return 1;
            if (!first) *buf++ = '.', buflen--;
            memcpy(buf, p + 1, *p);
            buf += *p;
            p += *p + 1;
        } 
        else if (*p >= 192 && p < end - 1) {
            size_t off = (*p & 0x3f) << 8 | p[1];
            if (off >= plen - 1) return 1;
            p = packet + off;
        } else return 1;
    }
    *buf = 0;
    return 0;
}
