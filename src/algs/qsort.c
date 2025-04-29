#include <stdlib.h>
#include <stdint.h>

static int wrap_cmp(const void *a, const void *b, void *c)
{
    int (*cmp)(const void *, const void *) = (int (*)(const void *, const void *))(uintptr_t)c;
    return cmp(a, b);
}

void qsort(void *b, size_t n, size_t sz, int (*cmp)(const void *, const void *))
{
    __qsort_r(b, n, sz, wrap_cmp, (void *)(uintptr_t)cmp);
}
