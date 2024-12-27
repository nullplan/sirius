#include <stdio.h>
#include <string.h>
#include "libc.h"

size_t fread(void *restrict vb, size_t sz, size_t n, FILE *restrict f)
{
    n *= sz;
    size_t rv = 0;
    unsigned char *buf = vb;
    __FLOCK(f);
    if (!__toread(f)) {
        while (n) {
            size_t tlen = MIN(n, f->end - f->pos);
            memcpy(buf, f->pos, tlen);
            f->pos += tlen;
            buf += tlen;
            n -= tlen;
            if (n) {
                ssize_t rd = f->read(f, buf, n);
                if (rd <= 0) break;
                buf += rd;
                n -= rd;
            }
        }
    }
    __FUNLOCK(f);
    return sz? rv / sz : 0;
}
