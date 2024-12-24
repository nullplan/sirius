#include <stdio.h>
#include <string.h>

int fputs(const char *restrict s, FILE *restrict f)
{
    int rv;
    size_t l = strlen(s);
    return fwrite(s, 1, l, f) == l? 0 : EOF;
}
