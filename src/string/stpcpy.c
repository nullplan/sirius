#include <string.h>

hidden char *__stpcpy(char *restrict dst, const char *restrict src)
{
    while ((*dst++ = *src++));
    return dst - 1;
}
weak_alias(stpcpy, __stpcpy);
