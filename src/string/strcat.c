#include <string.h>
char *strcat(char *restrict dst, const char *restrict src)
{
    char *r = dst;
    while (*dst) dst++;
    while ((*dst++ = *src++));
    return r;
}
