#include <string.h>
#include <stdlib.h>

char *strndup(const char *s, size_t n)
{
    const char *eos = memchr(s, 0, n);
    if (eos) n = eos - s;
    char *r = malloc(n + 1);
    if (r) {
        memcpy(r, s, n);
        r[n] = 0;
    }
    return r;
}
