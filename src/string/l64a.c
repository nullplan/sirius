#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static const char alphabet[64] = "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
char *l64a(long val)
{
    static char buf[7];
    char *p = buf;
    uint32_t x = val;
    while (x) {
        *p++ = alphabet[x & 63];
        x >>= 6;
    }
    *p = 0;
    return buf;
}
 

long a64l(const char *s) {
    uint32_t x = 0;
    for (size_t i = 0; i < 6 && s[i]; i++) {
        const char *p = memchr(alphabet, s[i], 64);
        if (!p) break;
        x |= (size_t)(p - alphabet) << (6 * i);
    }
    return x;
}
