#include <stdlib.h>

void *(bsearch)(const void *key, const void *base, size_t n, size_t sz, int (*cmp)(const void *, const void *))
{
    const unsigned char *b = base;
    while (n) {
        size_t mid = n / 2;
        int c = cmp(key, b + mid * sz);
        if (c == 0) return (void *)(b + mid * sz);
        if (c < 0) n = mid;
        else {
            b += (mid + 1) * sz;
            n -= mid + 1;
        }
    }
    return 0;
}
