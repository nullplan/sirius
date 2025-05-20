#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

static ssize_t getdelim_core(char **restrict ps, size_t *restrict pn, int delim, FILE *restrict f)
{
    if (!ps || !pn) {
        errno = EINVAL;
        f->flags |= F_ERR;
        return -1;
    }

    if (f->flags & F_EOF)
        return -1;

    char *string = *ps;
    size_t capacity = string? *pn : 0;
    size_t len = 0;

    for (;;) {
        int c = getc_unlocked(f);
        if (c == EOF && !len) return -1;
        if (c == EOF) break;

        if (len == capacity) {
            size_t attempt = capacity < 8? 16 : 2 * capacity;
            char *alloc = realloc(string, attempt);
            if (!alloc) {
                f->flags |= F_ERR;
                errno = ENOMEM;
                return -1;
            }
            *ps = string = alloc;
            *pn = capacity = attempt;
        }
        string[len++] = c;
        if (c == delim) break;
    }
    if (len == capacity) {
        size_t attempt = capacity + 1;
        char *alloc = realloc(string, attempt);
        if (!alloc) {
            f->flags |= F_ERR;
            errno = ENOMEM;
            return -1;
        }
        *ps = string = alloc;
        *pn = attempt;
    }
    string[len] = 0;
    return len;
}

ssize_t getdelim(char **restrict ps, size_t *restrict pn, int delim, FILE *restrict f)
{
    __FLOCK(f);
    ssize_t rv = getdelim_core(ps, pn, delim, f);
    __FUNLOCK(f);
    return rv;
}
