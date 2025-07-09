#include <wchar.h>
#include <stdio.h>
#include <pthread.h>

wint_t fgetwc(FILE *f)
{
    wint_t rv = WEOF;
    mbstate_t mbs = {0};
    wchar_t wc;
    __FLOCK(f);
    if (!__toread(f)
            && !(f->flags & F_EOF)
            && fwide(f, 1) > 0) {
        locale_t *ploc = &__pthread_self()->locale;
        locale_t oldloc = *ploc;
        *ploc = f->locale;
        for (;;) {
            int c = getc_unlocked(f);
            if (c == EOF) break;
            size_t mbl = mbrtowc(&wc, &(char){c}, 1, &mbs);
            if (mbl == -1) {
                f->flags |= F_ERR;
                break;
            }
            if (mbl != -2) {
                rv = wc;
                break;
            }
        }
        *ploc = oldloc;
    }
    __FUNLOCK(f);
    return rv;
}
weak_alias(getwc, fgetwc);
