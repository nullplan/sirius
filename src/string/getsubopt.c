#include <stdlib.h>
#include <string.h>

int getsubopt(char **restrict popt, char *const *restrict key, char **restrict value)
{
    char *opt = *popt;
    char *end = opt + __stridx(opt, ',');
    char *eq = memchr(opt, '=', end - opt);
    if (eq) *value = eq + 1;
    else *value = 0;
    size_t optlen = eq? eq - opt : end - opt;
    if (*end) {
        *end = 0;
        *popt = end + 1;
    } else
        *popt = end;
    for (int i = 0; key[i]; i++)
    {
        if (strlen(key[i]) == optlen && !memcmp(opt, key[i], optlen))
            return i;
    }
    return -1;
}
