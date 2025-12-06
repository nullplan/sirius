#include <string.h>
#include <stdint.h>
#include "libc.h"

static char *twobyte_strstr(const unsigned char *hs, const unsigned char *ns)
{
    uint16_t n = ns[0] << 8 | ns[1];
    uint16_t h = *hs++;
    while (*hs) {
        h = h << 8 | *hs++;
        if (h == n)
            return (void *)(hs - 2);
    }
    return 0;
}

static char *threebyte_strstr(const unsigned char *hs, const unsigned char *ns)
{
    uint32_t n = (ns[0] + 0ul) << 24 | ns[1] << 16 | ns[2] << 8;
    uint32_t h = hs[0] << 16 | hs[1] << 8;
    hs += 2;
    while (*hs) {
        h = (h | *hs++) << 8;
        if (h == n)
            return (void *)(hs - 3);
    }
    return 0;
}

static char *fourbyte_strstr(const unsigned char *hs, const unsigned char *ns)
{
    uint32_t n = (ns[0] + 0ul) << 24 | ns[1] << 16 | ns[2] << 8 | ns[3];
    uint32_t h = hs[0] << 16 | hs[1] << 8 | hs[2];
    hs += 3;
    while (*hs) {
        h = h << 8 | *hs++;
        if (h == n)
            return (void *)(hs - 4);
    }
    return 0;
}

struct factorization {
    size_t ms;
    size_t p;
};

static struct factorization maximum_suffix(const unsigned char *n, size_t nl,
        int rev)
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

static char *twoway_strstr(const unsigned char *h, const unsigned char *n)
{
    size_t bitset[256/8/sizeof (size_t)] = {0};
    size_t shifttab[256];
    size_t nl;
    for (nl = 0; h[nl] && n[nl]; nl++) {
        BITOP(bitset, |=, n[nl]);
        shifttab[n[nl]] = nl + 1;
    }
    if (n[nl]) return 0;

    struct factorization crit = maximum_suffix(n, nl, 0);
    struct factorization rev = maximum_suffix(n, nl, 1);
    if (rev.ms > crit.ms)
        crit = rev;

    size_t mem0 = 0;
    size_t mem = 0;
    if (!memcmp(n, n + crit.p, crit.ms)) {
        mem0 = nl - crit.p;
    } else {
        crit.p = MAX(crit.ms, nl - crit.ms - 1) + 1;
    }

    const unsigned char *z = h;
    for (;;) {
        if (z - h < nl) {
            size_t grow = nl | 63;
            const unsigned char *z2 = memchr(z, 0, grow);
            if (!z2) z += grow;
            else if (z2 - h < nl) return 0;
            else z = z2;
        }

        size_t k = nl - (BITOP(bitset, &, h[nl-1])? shifttab[h[nl-1]] : 0);
        if (k) {
            h += k;
            mem = 0;
            continue;
        }

        for (k = MAX(mem, crit.ms); k < nl && h[k] == n[k]; k++);
        if (k < nl) {
            h += k - crit.ms + 1;
            mem = 0;
            continue;
        }
        for (k = crit.ms; k > mem && h[k-1] == n[k-1]; k--);
        if (k <= mem) return (char *)h;
        h += crit.p;
        mem = mem0;
    }
}

char *(strstr)(const char *h, const char *n)
{
    if (!*n) return (char *)h;
    h += __stridx(h, *n);
    if (!h[0]) return 0;
    if (!n[1]) return (char *)h;
    if (!h[1]) return 0;
    if (!n[2]) return twobyte_strstr((const void *)h, (const void *)n);
    if (!h[2]) return 0;
    if (!n[3]) return threebyte_strstr((const void *)h, (const void *)n);
    if (!h[3]) return 0;
    if (!n[4]) return fourbyte_strstr((const void *)h, (const void *)n);
    if (!h[4]) return 0;
    return twoway_strstr((const void *)h, (const void *)n);
}
