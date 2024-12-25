#include <string.h>

int strncmp(const char *ca, const char *cb, size_t n)
{
    const unsigned char *a = (const void *)ca;
    const unsigned char *b = (const void *)cb;
    while (n && *a == *b && *a) {
        a++;
        b++;
        n--;
    }
    if (!n) return 0;
    return *a - *b;
}
