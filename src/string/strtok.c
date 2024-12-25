#include <string.h>

char *strtok(char *restrict str, const char *restrict tok)
{
    static char *state;
    return __strtok_r(str, tok, &state);
}
