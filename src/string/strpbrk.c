#include <string.h>

char *(strpbrk)(const char *str, const char *set)
{
    size_t l = strcspn(str, set);
    if (str[l]) return (void *)(str + l);
    return 0;
}
