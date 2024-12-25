#include <string.h>

char *(strrchr)(const char *str, int c)
{
    size_t i = __strridx(str, c);
    if ((unsigned char)(str[i]) == c)
        return (void *)(str + i);
    return 0;
}
