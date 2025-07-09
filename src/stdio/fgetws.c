#include <wchar.h>
#include <stdio.h>
#include <errno.h>

wchar_t *fgetws(wchar_t *restrict ws, int n, FILE *restrict f)
{
    wchar_t *rv = ws;
    __FLOCK(f);
    if (f->flags & F_EOF)
        rv = 0;
    else if (n <= 0) {
        errno = EINVAL;
        rv = 0;
    } else {
        n--;
        while (n) {
            wint_t wc = fgetwc(f);
            if (wc == WEOF) {
                if (ws == rv || (f->flags & F_ERR))
                    rv = 0;
                break;
            }
            *ws++ = wc;
            n--;
            if (wc == L'\n') break;
        }
        if (rv) *ws = 0;
    }
    __FUNLOCK(f);
    return rv;
}
