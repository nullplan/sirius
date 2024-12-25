#include <string.h>

char *(strchr)(const char *s, int c)
{
    s += __stridx(s, c);
    if (!*s) return 0;
    return (void *)s;
}
