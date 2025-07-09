#include <wchar.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <pthread.h>

wint_t fputwc(wchar_t wc, FILE *f)
{
    wint_t rv = WEOF;
    char mbbuf[MB_LEN_MAX];
    __FLOCK(f);
    if (!__towrite(f) && fwide(f, 1) > 0)
    {
        locale_t *ploc = &__pthread_self()->locale;
        locale_t oldloc = *ploc;
        *ploc = f->locale;
        int mbl = wctomb(mbbuf, wc);
        if (mbl > 0 && __fwritex(mbbuf, mbl, f) == mbl)
            rv = wc;
        *ploc = oldloc;
    }
    __FUNLOCK(f);
    return rv;
}
weak_alias(putwc, fputwc);
