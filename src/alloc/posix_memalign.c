#include <stdlib.h>
#include <errno.h>

int posix_memalign(void **pp, size_t a, size_t sz)
{
    if ((a & (a - 1)) || a < sizeof (void *)) {
        errno = EINVAL;
        return -1;
    }
    void *p = aligned_alloc(a, sz);
    if (!p) return -1;
    *pp = p;
    return 0;
}
