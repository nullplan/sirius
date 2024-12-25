#include <string.h>
int strcmp(const char *ca, const char *cb)
{
    const unsigned char *a = (const void *)ca;
    const unsigned char *b = (const void *)cb;
    while (*a == *b && *a) a++, b++;
    return *a - *b;
}
