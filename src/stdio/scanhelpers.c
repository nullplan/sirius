#include <stdio.h>

hidden int __shlim(FILE *f, off_t lim)
{
    if (__toread(f)) return EOF;
    f->shcnt = 0;
    f->shlim = lim;
    f->shend = lim && (f)->end - (f)->pos <= lim? (f)->pos + lim : (f)->end;
    return 0;
}

hidden int __shgetc(FILE *f)
{
    if (f->shlim && shcnt(f) == f->shlim) return EOF;
    if (f->pos != f->shend) return *f->pos++;
    off_t cnt = shcnt(f);
    int rv = __underflow(f);
    if (rv != EOF) {
        f->shcnt = cnt;
        f->shend = f->shlim && f->end - f->pos <= f->shlim? f->pos + f->shlim : f->end;
    }
    return rv;
}
