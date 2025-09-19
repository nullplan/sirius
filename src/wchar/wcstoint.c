#include <wchar.h>
#include <inttypes.h>
#include <stdio.h>
#include <limits.h>

/* this is a massive cheat:
 * I convert all wchar to bytes because all interesting characters for these functions
 * are ASCII characters.
 *
 * For that reason, I know that every byte consumed is exactly one wchar advanced.
 */
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

static uintmax_t wcstoint(const wchar_t *restrict ws, wchar_t **restrict endp, int base, uintmax_t limit)
{
    FILE f;
    unsigned char buffer[80];
    f.buf = buffer;
    f.buf_size = sizeof buffer;
    f.dir = 0;
    f.flags = 0;
    f.read = wstr_read;
    f.lock = -1;
    uintmax_t rv = __intscan(&f, base, limit);
    if (endp)
        *endp = (void *)(ws + shcnt(&f));
    return rv;
}

long wcstol(const wchar_t *restrict ws, wchar_t **restrict endp, int base)
{
    return wcstoint(ws, endp, base, LONG_MIN);
}

long long wcstoll(const wchar_t *restrict ws, wchar_t **restrict endp, int base)
{
    return wcstoint(ws, endp, base, LLONG_MIN);
}

unsigned long wcstoul(const wchar_t *restrict ws, wchar_t **restrict endp, int base)
{
    return wcstoint(ws, endp, base, ULONG_MAX);
}

unsigned long long wcstoull(const wchar_t *restrict ws, wchar_t **restrict endp, int base)
{
    return wcstoint(ws, endp, base, ULLONG_MAX);
}

intmax_t wcstoimax(const wchar_t *restrict ws, wchar_t **restrict endp, int base)
{
    return wcstoint(ws, endp, base, INTMAX_MIN);
}

uintmax_t wcstoumax(const wchar_t *restrict ws, wchar_t **restrict endp, int base)
{
    return wcstoint(ws, endp, base, UINTMAX_MAX);
}
