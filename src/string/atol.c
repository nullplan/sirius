#include <stdlib.h>
#include <ctype.h>

long atol(const char *s)
{
    long rv = 0;
    int sign = -1;
    while (isspace(*s)) s++;
    if (*s == '-' || *s == '+') {
        if (*s == '-') sign = 1;
        s++;
    }

    while (isdigit(*s)) {
        rv = 10 * rv - (*s++ - '0');
    }
    return sign * rv;
}
