#include <stdio.h>
#include <string.h>
#include "libc.h"

struct string {
    char *p;
    size_t n;
};
static size_t string_write(FILE *f, const void *b, size_t sz)
{
    struct string *str = f->cookie;
    size_t tlen = MIN(str->n, f->pos - f->buf);
    memcpy(str->p, f->buf, tlen);
    str->p += tlen;
    str->n -= tlen;
    tlen = MIN(str->n, sz);
    memcpy(str->p, b, tlen);
    str->p += tlen;
    str->n -= tlen;
    *str->p = 0;
    f->pos = f->buf;
    f->end = f->buf + f->buf_size;
    return sz;
}

int vsnprintf(char *restrict s, size_t n, const char *restrict fmt, va_list ap)
{
    char dummy;
    if (!n) {
        s = &dummy;
        n = 1;
    }
    struct string str = {s, n - 1};
    FILE f;
    f.flags = 0;
    f.buf = (void *)&dummy;
    f.buf_size = 0;
    f.dir = 0;
    f.write = string_write;
    f.lock = -1;
    f.cookie = &str;
    return vfprintf(&f, fmt, ap);
}
