#include <string.h>

char *strcpy(char *restrict dst, const char *restrict src)
{
    __stpcpy(dst, src);
    return dst;
}
