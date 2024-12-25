#include <string.h>
#include <stdlib.h>

char *strdup(const char *s)
{
    size_t l = strlen(s);
    char *r = malloc(l + 1);
    if (!r) return 0;
    return memcpy(r, s, l + 1);
}
