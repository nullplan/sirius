#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

wint_t ungetwc(wint_t wc, FILE *f)
{
    wint_t rv = WEOF;
    __FLOCK(f);
    if (wc != WEOF && !__toread(f) && fwide(f, 1) > 0) {
        locale_t *ploc = &__pthread_self()->locale;
        locale_t oldloc = *ploc;
        *ploc = f->locale;
        char mbbuf[MB_LEN_MAX];
        int mbl = wctomb(mbbuf, wc);
        if (mbl > 0 && f->pos - f->buf + UNGET <= mbl) {
            f->pos = memcpy(f->pos - mbl, mbbuf, mbl);
            f->flags &= ~F_EOF;
        }
        *ploc = oldloc;
    }
    __FUNLOCK(f);
    return rv;
}
