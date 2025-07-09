#include <wchar.h>
#include <stdio.h>

int fputws(const wchar_t *restrict ws, FILE *restrict f)
{
    int rv = 0;
    __FLOCK(f);
    while (*ws) {
        if (fputwc(*ws++, f) == WEOF) {
            rv = EOF;
            break;
        }
    }
    __FUNLOCK(f);
    return rv;
}
