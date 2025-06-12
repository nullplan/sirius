#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

static char *memclear(char *p, size_t n)
{
    #ifdef __GNUC__
    if (n >= 2 * sizeof (size_t)) {
        typedef size_t __attribute__((may_alias)) word_t;
        char *end = p + n;
        if ((uintptr_t)end & (sizeof (size_t) - 1)) {
            size_t len = (uintptr_t)end & (sizeof (size_t) - 1);
            end = memset(end - len, 0, len);
            n -= len;
        }
        word_t *wp = (void *)end;
        while (n >= sizeof *wp) {
            if (wp[-1])
                wp[-1] = 0;
            wp--;
            n -= sizeof *wp;
        }
    }
    #endif

    return memset(p, 0, n);
}

void *calloc(size_t a, size_t b)
{
    if (a > PTRDIFF_MAX/b) {
        errno = ENOMEM;
        return 0;
    }
    if (!a || !b) {
        errno = EINVAL;
        return 0;
    }

    size_t n = a * b;
    void *p = malloc(n);
    if (!p) return p;
    return memclear(p, n);
}
