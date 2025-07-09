#include <wchar.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

struct wstring {
    wchar_t *s;
    size_t n;
    int wr;
};

static void widen(struct wstring *ws, const unsigned char *data, size_t len)
{
    if (ws->wr >= 0) {
        while (len && ws->n) {
            size_t mbl = mbtowc(ws->s, (char *)data, len);
            if (mbl == -1) {
                errno = EILSEQ;
                ws->wr = -1;
                return;
            }
            ws->s++;
            ws->n--;
            ws->wr++;
            data += mbl;
            len -= mbl;
        }
        /* no break in above loop -> here we have !len || !ws->n */
        if (len) {
            errno = EOVERFLOW;
            ws->wr = -1;
        }
    }
}

static size_t ws_write(FILE *f, const void *b, size_t len)
{
    struct wstring *ws = f->cookie;
    widen(ws, f->buf, f->pos - f->buf);
    widen(ws, b, len);
    *ws->s = 0;
    return len;
}

int vswprintf(wchar_t *restrict s, size_t n, const wchar_t *restrict fmt, va_list ap)
{
    unsigned char dummy;
    if (!n) {
        errno = EOVERFLOW;
        return -1;
    }
    if (n > INT_MAX) n = INT_MAX; /* can't write more than that without overflow */

    struct wstring ws = {s, n-1};
    FILE f;
    f.buf = &dummy;
    f.buf_size = 0;
    f.dir = 0;
    f.lbf = -1;
    f.lock = -1;
    f.cookie = &ws;
    f.flags = 0;
    f.write = ws_write;
    vfwprintf(&f, fmt, ap);
    return ws.wr;
}
