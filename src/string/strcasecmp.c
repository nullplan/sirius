#include <strings.h>
#include <wchar.h>
#include <wctype.h>
#include <stdlib.h>

static wint_t pop_wchar(const char **p)
{
    wchar_t wc;
    size_t l = mbtowc(&wc, *p, (size_t)-1);
    if (l == -1) {
        wc = TO_CODEUNIT(**p);
        l = 1;
    } else if (l == 0)
        l = 1;
    *p += l;
    return wc;
}

int strcasecmp(const char *a, const char *b)
{
    for (;;) {
        if (!*a) return -(unsigned char)*b;
        wint_t wa = towlower(pop_wchar(&a));
        wint_t wb = towlower(pop_wchar(&b));
        if (wa == wb) continue;
        if (wa > wb) return 1;
        return -1;
    }
}
