#include <stdio.h>
#include <string.h>
#include <errno.h>

char *fgets(char *restrict buf, int sz, FILE *restrict f)
{
    char *rv = buf;
    __FLOCK(f);
    if (f->flags & F_EOF)
        rv = 0;
    else if (sz <= 0) {
        errno = EINVAL;
        rv = 0;
    } else {
        sz--;
        while (sz) {
            int c = getc_unlocked(f);
            if (c == EOF) {
                rv = 0;
                break;
            }
            *buf++ = c;
            sz--;
            if (c == '\n') break;
            if (f->buf_size) {
                unsigned char *p = memchr(f->pos, '\n', f->end - f->pos);
                size_t tlen = p? p - f->pos + 1 : f->end - f->pos;
                if (tlen > sz) tlen = sz;
                memcpy(buf, f->pos, tlen);
                f->pos += tlen;
                buf += tlen;
                sz -= tlen;
                if (p) break;
            }
        }
        *buf = 0;
    }
    __FUNLOCK(f);
    return rv;
}
