#include <wchar.h>
#include "libc.h"

struct factorization {
    size_t ms;
    size_t p;
};

static struct factorization maximum_suffix(const wchar_t *n, size_t nl, int rev)
{
    size_t ip = -1;
    size_t jp = 0;
    size_t k = 1;
    size_t p = 1;
    while (jp+k < nl) {
        if (n[ip+k] == n[jp+k]) {
            if (k == p) {
                jp += k;
                k = 1;
            } else k++;
        } else if ((n[ip+k] < n[jp+k]) ^ rev) {
            ip = jp++;
            k = p = 1;
        } else {
            jp += k;
            k = 1;
            p = jp - ip;
        }
    }
    return (struct factorization){ip+1, p};
}

wchar_t *(wcsstr)(const wchar_t *h, const wchar_t *n)
{
    size_t nl;
    for (nl = 0; h[nl] && n[nl]; nl++);
    if (n[nl]) return 0;

    struct factorization crit, rev;
    crit = maximum_suffix(n, nl, 0);
    rev = maximum_suffix(n, nl, 1);
    if (rev.ms > crit.ms)
        crit = rev;

    size_t mem0 = 0;
    size_t mem = 0;
    if (!wmemcmp(n, n + crit.p, crit.ms))
        mem0 = nl - crit.p;
    else
        crit.p = MAX(crit.ms, nl - crit.ms - 1) + 1;

    const wchar_t *z = h;
    for (;;) {
        if (z - h < nl) {
            size_t grow = nl | 63;
            const wchar_t *z2 = wmemchr(z,0, grow);
            if (!z2) z += grow;
            else if (z2 - h < nl) return 0;
            else z = z2;
        }

        size_t k = MAX(mem, crit.ms);
        for (; k < nl && h[k] == n[k]; k++);
        if (k < nl) {
            h += k - crit.ms + 1;
            mem = 0;
            continue;
        }

        for (k = crit.ms; k > mem && h[k-1] == n[k-1]; k--);
        if (k <= mem) return (wchar_t *)h;
        h += crit.p;
        mem = mem0;
    }
}
