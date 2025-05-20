#include <stdio.h>

ssize_t getline(char **restrict ps, size_t *restrict pn, FILE *restrict f)
{
    return getdelim(ps, pn, '\n', f);
}
