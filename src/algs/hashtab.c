#include <search.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "cpu.h"

static ENTRY *tab;
static size_t mask;
static size_t n;

int hcreate(size_t x)
{
    const int nbit = 8 * sizeof (size_t);
    int ilogb = nbit - a_clz(x);

    if (ilogb == nbit) {
        errno = EOVERFLOW;
        return 0;
    }
    x = 1ul << (ilogb + 1);

    ENTRY *p = calloc(x, sizeof (ENTRY));
    if (!p) return 0;
    tab = p;
    mask = x - 1;
    n = 0;
    return 1;
}

void hdestroy(void)
{
    free(tab);
}

static size_t hash(const char *s)
{
    const unsigned char *us = (void *)s;
    size_t h = 5381;
    while (*us)
        h = 33 * h + *us++;
    return h;
}

ENTRY *hsearch(ENTRY e, ACTION a)
{
    size_t i = hash(e.key);
    for (size_t j = 1; tab[i & mask].key; i += j++)
        if (!strcmp(tab[i&mask].key, e.key))
            return tab + (i & mask);
    if (a == FIND) return 0;
    if (n == mask) return 0;
    n++;
    tab[i & mask] = e;
    return tab + (i & mask);
}
