#include <string.h>

void *memset_explicit(void *dst, int c, size_t n)
{
    void *rv = memset(dst, c, n);
    __asm__("" :: "m"(*(char *)dst));
    return rv;
}
