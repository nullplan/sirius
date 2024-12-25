#include <string.h>

char *strncpy(char *restrict dst, const char *restrict src, size_t n)
{
    __stpncpy(dst, src, n);
    return dst;
}
