#include <stdio.h>
#include <string.h>

hidden size_t __fwritex(const void *restrict buf, size_t n, FILE *restrict f)
{
    if (__towrite(f)) return 0;
    /* C defines fwrite() in terms of fputc(). The result of this function
     * should be the same as calling fputc() on every input byte.
     *
     * I am assuming that f->write has no side effects besides writing the
     * buffer and the given data to the file, though.
     *
     * For unbuffered files, the effect of calling fputc() on every byte is
     * that every byte lands in the file.
     *
     * For fully buffered files, the effect is that every time the buffer
     * fills, it is written to the file. In the end, an initial portion
     * of the given data is written immediately, and a remainder is buffered.
     *
     * For line buffered files, the portion of "buf" before the last line break
     * is immediately written, and the remainder is treated the same as the
     * fully buffered case.
     */
    if (!f->buf_size) return f->write(f, buf, n);
    size_t rv = 0;
    if (f->lbf > 0) {
        const unsigned char *c = buf;
        size_t lastbreak = 0;
        for (size_t i = 0; i < n; i++)
            if (c[i] == '\n')
                lastbreak = i;
        if (c[lastbreak] == '\n') {
            rv = f->write(f, buf, lastbreak + 1);
            buf = c + lastbreak + 1;
            n -= lastbreak + 1;
        }
    }
    if (n <= f->end - f->pos) {
        memcpy(f->pos, buf, n);
        f->pos += n;
        return rv + n;
    }
    size_t rem = (f->pos - f->buf + n) % f->buf_size;
    rv += f->write(f, buf, n - rem);
    if (!(f->flags & F_ERR)) {
        memcpy(f->buf, (const char *)buf + n - rem, rem);
        f->pos = f->buf + rem;
        rv += rem;
    }
    return rv;
}

size_t fwrite(const void *restrict buf, size_t sz, size_t n, FILE *restrict f)
{
    size_t rv = 0;
    __FLOCK(f);
    rv = __fwritex(buf, n * sz, f);
    __FUNLOCK(f);
    return sz? rv/sz : 0;
}
