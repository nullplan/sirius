#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

struct string {
    char *s;
    size_t n;
};

static void string_copy(struct string *s, const unsigned char *b, size_t len)
{
    if (!s->n) return;
    if (len > s->n)
        len = s->n;
    memcpy(s->s, b, len);
    s->s += len;
    s->n -= len;
}

static size_t string_write(FILE *f, const void *b, size_t len)
{
    struct string *s = f->cookie;
    string_copy(s, f->buf, f->pos - f->buf);
    string_copy(s, b, len);
    f->pos = f->buf;
    *s->s = 0;
    return len;
}

static int getint(const char *restrict *ps)
{
    const char *s = *ps;
    int rv = 0;
    while (isdigit(*s))
        rv = 10 * rv + *s++ - '0';
    *ps = s;
    return rv;
}

static int strfromx(char *restrict s, size_t n, const char *restrict fmt, long double val, enum format type)
{
    int prec = -1;
    if (*fmt != '%') {
        errno = EINVAL;
        return -1;
    }
    fmt++;
    int c = *fmt++;
    if (c == '.') {
        prec = getint(&fmt);
        c = *fmt++;
    }
    switch (c|32) {
        case 'a':
        case 'e':
        case 'f':
        case 'g':
            break;
        default:
            errno = EINVAL;
            return -1;
    }

    char dummy;
    /* avoid an indirect call for every single byte */
    unsigned char buffer[80];
    if (!n) {
        n = 1;
        s = &dummy;
    }
    struct string str = {s, n-1};
    FILE f;
    f.buf = buffer;
    f.buf_size = sizeof buffer;
    f.dir = 0;
    f.flags = 0;
    f.lbf = -1;
    f.write = string_write;
    f.cookie = &str;
    f.lock = -1;
    f.orientation = 0;

    size_t rv = __fmt_fp(&f, val, 0, prec, 0, c, type);
    string_write(&f, 0, 0);
    if (rv > INT_MAX) {
        errno = EOVERFLOW;
        return -1;
    }
    return rv;
}

int strfromf(char *restrict s, size_t n, const char *restrict fmt, float val)
{
    return strfromx(s, n, fmt, val, fmt_float);
}

int strfromd(char *restrict s, size_t n, const char *restrict fmt, double val)
{
    return strfromx(s, n, fmt, val, fmt_double);
}

int strfroml(char *restrict s, size_t n, const char *restrict fmt, long double val)
{
    return strfromx(s, n, fmt, val, fmt_ldouble);
}
