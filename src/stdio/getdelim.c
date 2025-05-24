#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

struct dynstr {
    char *buf;
    size_t capacity;
    size_t len;
};

static int push_char(struct dynstr *ds, char **ps, size_t *pn, int c, int islast)
{
    if (ds->len == ds->capacity)
    {
        size_t attempt = islast? ds->capacity + 1 :
            ds->capacity < 11? 16 :
            ds->capacity + ds->capacity / 2;
        char *alloc = realloc(ds->buf, attempt);
        if (!alloc) {
            errno = ENOMEM;
            return -1;
        }
        *ps = ds->buf = alloc;
        *pn = ds->capacity = attempt;
    }
    ds->buf[ds->len] = c;
    if (!islast) ds->len++;
    return 0;
}

static ssize_t getdelim_core(char **restrict ps, size_t *restrict pn, int delim, FILE *restrict f)
{
    if (!ps || !pn) {
        errno = EINVAL;
        goto error;
    }

    if (f->flags & F_EOF)
        return -1;

    struct dynstr ds = {*ps, *ps? 0 : *pn, 0};
    int olderr = f->flags & F_ERR;
    f->flags &= ~F_ERR;

    for (;;) {
        int c = getc_unlocked(f);
        if (c == EOF && !ds.len) return -1;
        if (c == EOF) break;

        if (push_char(&ds, ps, pn, c, 0)) goto error;
        if (c == delim) break;
    }
    if (f->flags & F_ERR) return -1;
    if (push_char(&ds, ps, pn, 0, 1)) goto error;
    f->flags |= olderr;
    return ds.len;

error:
    f->flags |= F_ERR;
    return -1;
}

ssize_t getdelim(char **restrict ps, size_t *restrict pn, int delim, FILE *restrict f)
{
    __FLOCK(f);
    ssize_t rv = getdelim_core(ps, pn, delim, f);
    __FUNLOCK(f);
    return rv;
}
