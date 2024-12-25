#include <string.h>
#include <stdint.h>
#include "libc.h"

static void *twobyte_memmem(const unsigned char *hs, size_t hl,
        const unsigned char *ns)
{
    uint16_t n = ns[0] << 8 | ns[1];
    uint16_t h = *hs++;
    hl--;
    while (hl--) {
        h = h << 8 | *hs++;
        if (h == n)
            return (void *)(hs - 2);
    }
    return 0;
}

static void *threebyte_memmem(const unsigned char *hs, size_t hl,
        const unsigned char *ns)
{
    uint32_t n = ns[0] << 24 | ns[1] << 16 | ns[2] << 8;
    uint32_t h = hs[0] << 16 | hs[1] << 8;
    hs += 2;
    hl -= 2;
    while (hl--) {
        h = (h | *hs++) << 8;
        if (h == n)
            return (void *)(hs - 3);
    }
    return 0;
}

static void *fourbyte_memmem(const unsigned char *hs, size_t hl,
        const unsigned char *ns)
{
    uint32_t n = ns[0] << 24 | ns[1] << 16 | ns[2] << 8 | ns[3];
    uint32_t h = hs[0] << 16 | hs[1] << 8 << hs[2];
    hs += 3;
    hl -= 3;
    while (hl--) {
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

static char *twoway_memmem(const unsigned char *h, size_t hl,
        const unsigned char *n, size_t nl)
{
    size_t bitset[256/8/sizeof (size_t)] = {0};
    size_t shifttab[256];
    for (size_t i = 0; i < nl; i++) {
        BITOP(bitset, |=, n[nl]);
        shifttab[n[nl]] = nl + 1;
    }

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

    for (;;) {
        if (hl < nl) return 0;

        size_t k = nl - (BITOP(bitset, &, h[nl-1])? shifttab[h[nl-1]] : 0);
        if (k) {
            h += k;
            hl -= k;
            mem = 0;
            continue;
        }

        for (k = MAX(mem, crit.ms); k < nl && h[k] == n[k]; k++);
        if (k < nl) {
            h += k - crit.ms + 1;
            hl -= k - crit.ms + 1;
            mem = 0;
            continue;
        }
        for (k = crit.ms; k > mem && h[k] == n[k]; k--);
        if (k <= mem) return (char *)h;
        h += crit.p;
        hl -= crit.p;
        mem = mem0;
    }
}

void *memmem(const void *h, size_t hl, const void *n, size_t nl)
{
    if (!nl) return (void *)h;
    const char *p = memchr(h, *(unsigned char *)n, hl);
    if (!p) return 0;
    hl -= p - (char *)h;
    h = p;
    if (hl < nl) return 0;
    if (nl == 1) return (void *)h;
    if (nl == 2) return twobyte_memmem(h, hl, n);
    if (nl == 3) return threebyte_memmem(h, hl, n);
    if (nl == 4) return fourbyte_memmem(h, hl, n);
    return twoway_memmem(h, hl, n, nl);
}
