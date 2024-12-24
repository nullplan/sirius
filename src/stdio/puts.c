#include <stdio.h>
#include <string.h>

int puts(const char *s)
{
    int rv = EOF;
    __FLOCK(stdout);
    size_t l = strlen(s);
    if (__fwritex(s, l, stdout) == l)
        rv = putc_unlocked('\n', stdout);
    __FUNLOCK(stdout);
    return rv;
}
