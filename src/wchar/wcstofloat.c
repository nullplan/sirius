/* see wcstoint.c for justifications */
#include <wchar.h>
#include <inttypes.h>
#include <stdio.h>
#include <limits.h>
#include <float.h>
static size_t narrow(unsigned char *d, size_t n, const wchar_t *ws)
{
    size_t rv = 0;
    while (n && *ws)
    {
        int b = wctob(*ws);
        if (b == EOF) break;
        *d++ = b;
        ws++;
        rv++;
        n--;
    }
    return rv;
}
static ssize_t wstr_read(FILE *f, void *data, size_t n)
{
    wchar_t *ws = f->cookie;
    size_t rv = narrow(data, n, ws);
    ws += rv;
    size_t buffered = narrow(f->buf, f->buf_size, ws);
    ws += buffered;
    f->pos = f->buf;
    f->end = f->buf + buffered;
    f->cookie = ws;
    return rv;
}

static long double wcstofloat(const wchar_t *restrict ws, wchar_t **restrict endp, int bits, int emin)
{
    FILE f;
    unsigned char buffer[80];
    f.buf = buffer;
    f.buf_size = sizeof buffer;
    f.dir = 0;
    f.flags = 0;
    f.read = wstr_read;
    f.lock = -1;
    f.cookie = (void *)ws;
    shlim(&f, 0);
    long double rv = __floatscan(&f, 1, bits, emin);
    if (endp)
        endp = (void *)(ws + shcnt(&f));
    return rv;
}

float wcstof(const wchar_t *restrict ws, wchar_t **restrict endp)
{
    return wcstofloat(ws, endp, FLT_MANT_DIG, FLT_MIN_EXP);
}

double wcstod(const wchar_t *restrict ws, wchar_t **restrict endp)
{
    return wcstofloat(ws, endp, DBL_MANT_DIG, DBL_MIN_EXP);
}

long double wcstold(const wchar_t *restrict ws, wchar_t **restrict endp)
{
    return wcstofloat(ws, endp, LDBL_MANT_DIG, LDBL_MIN_EXP);
}
