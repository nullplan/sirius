#include <string.h>

hidden char *__strtok_r(char *restrict str, const char *restrict tok,
        char **restrict st)
{
    if (!str) str = *st;
    str += strspn(str, tok);
    if (!*str) return 0;
    char *rv = str;
    str += strcspn(str, tok);
    if (*str) {
        *str = 0;
        *st = str + 1;
    } else
        *st = str;
    return rv;
}
weak_alias(strtok_r, __strtok_r);
