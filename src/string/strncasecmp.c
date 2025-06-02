#include <strings.h>
#include <wchar.h>
#include <wctype.h>
#include <stdlib.h>

static wint_t pop_wchar(const char **p, size_t *n)
{
    if (!*n) return 0;

    wchar_t wc;
    size_t l = mbtowc(&wc, *p, *n);
    if (l == -1) {
        wc = TO_CODEUNIT(**p);
        l = 1;
    } else if (l == 0)
        l = 1;
    *p += l;
    *n -= l;
    return wc;
}

int strncasecmp(const char *a, const char *b, size_t na)
{
    size_t nb = na;
    for (;;) {
        if (!na) return -!!nb;
        wint_t wa = towlower(pop_wchar(&a, &na));
        wint_t wb = towlower(pop_wchar(&b, &nb));
        if (wa == wb) continue;
        if (wa > wb) return 1;
        return -1;
    }
}
