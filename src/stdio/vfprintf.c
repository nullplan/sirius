#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <wchar.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include "libc.h"

static void out(FILE *f, const void *p, size_t l)
{
    if (f && l) __fwritex(p, l, f);
}
static int addo(int *x, size_t a)
{
    if (a > INT_MAX || *x > INT_MAX - a) return 1;
    *x += a;
    return 0;
}

static int getint(const char *restrict *ps)
{
    int rv = 0;
    const char *s = *ps;
    while (*s - '0' < 10u)
        rv = 10 * rv + *s++ - '0';
    *ps = s;
    return rv;
}

static char *fmt_b(char *z, uintmax_t x)
{
    for (       ; x > ULONG_MAX; x >>= 1) *--z = '0' + (x & 1);
    for (unsigned long y = x; y; y >>= 1) *--z = '0' + (y & 1);
    return z;
}

static char *fmt_o(char *z, uintmax_t x)
{
    for (       ; x > ULONG_MAX; x >>= 3) *--z = '0' + (x & 7);
    for (unsigned long y = x; y; y >>= 3) *--z = '0' + (y & 7);
    return z;
}

static char *fmt_x(char *z, uintmax_t x, int lcase)
{
    const char *const alpha = lcase? "0123456789abcdef" : "0123456789ABCDEF";
    for (       ; x > ULONG_MAX; x >>= 4) *--z = alpha[x & 15];
    for (unsigned long y = x; y; y >>= 4) *--z = alpha[y & 15];
    return z;
}

static char *fmt_d(char *z, uintmax_t x)
{
    for (       ; x > ULONG_MAX; x /= 10) *--z = '0' + (x % 10);
    for (unsigned long y = x; y; y /= 10) *--z = '0' + (y % 10);
    return z;
}

static void pad(FILE *f, int c, int w, int len, int flags)
{
    char buf[64];
    if (len >= w || (flags & (FLG_LEFT | FLG_ZERO))) return;
    len = w - len;
    memset(buf, c, MIN(len, sizeof buf));
    while (len > sizeof buf)
        out(f, buf, sizeof buf), len -= sizeof buf;
    out(f, buf, len);
}

enum {
    BARE, HPRE, HHPRE, LPRE, LLPRE, JPRE, ZTPRE, CLPRE,
    WPRE, W8PRE, W1PRE, W3PRE, W6PRE, W16PRE, W32PRE, W64PRE,
    WFPRE, WF8PRE, WF1PRE, WF3PRE, WF6PRE, WF16PRE, WF32PRE, WF64PRE,
    END,
    NOTHING,
    SCHAR, SHORT, INT, LONG, LLONG, PTRDIFF, WINT, INTMAX,
    INT8, INT16, INT32, INT64,
    INT_FAST8, INT_FAST16, INT_FAST32, INT_FAST64,
    UCHAR, USHORT, UINT, ULONG, ULLONG, SIZE, UINTMAX,
    UINT8, UINT16, UINT32, UINT64,
    UINT_FAST8, UINT_FAST16, UINT_FAST32, UINT_FAST64,
    DBL, LDBL,
    PSCHAR, PSHORT, PINT, PLONG, PLLONG, PPTRDIFF, PWCHAR, PCHAR,
    PVOID, PINTMAX,
};

union arg {
    uintmax_t u;
    intmax_t i;
    long double f;
    void *p;
};

#define S(x)    [(x) - '0']
static const unsigned char state_machine[END]['z'-'0'+1] = {
    [BARE] = {
        S('h') = HPRE, S('l') = LPRE, S('j') = JPRE, S('z') = ZTPRE,
        S('t') = ZTPRE, S('L') = CLPRE, S('w') = WPRE,

        S('d') = INT, S('i') = INT, S('o') = UINT, S('u') = UINT, S('x') = UINT,
        S('X') = UINT, S('b') = UINT, S('B') = UINT, S('n') = PINT,

        S('a') = DBL, S('e') = DBL, S('f') = DBL, S('g') = DBL,
        S('A') = DBL, S('E') = DBL, S('F') = DBL, S('G') = DBL,

        S('c') = INT, S('s') = PCHAR,

        S('p') = PVOID, S('m') = NOTHING,
        S('C') = WINT, S('S') = PWCHAR,
    },
    [HPRE] = {
        S('h') = HHPRE,

        S('d') = SHORT, S('i') = SHORT, S('o') = USHORT, S('u') = USHORT,
        S('x') = USHORT, S('X') = USHORT, S('b') = USHORT, S('B') = USHORT,
        S('n') = PSHORT,
    },
    [HHPRE] = {
        S('d') = SCHAR, S('i') = SCHAR, S('o') = UCHAR, S('u') = UCHAR,
        S('x') = UCHAR, S('X') = UCHAR, S('b') = UCHAR, S('B') = UCHAR,
        S('n') = PSCHAR,
    },
    [LPRE] = {
        S('l') = LLPRE,

        S('d') = LONG, S('i') = LONG, S('o') = ULONG, S('u') = ULONG,
        S('x') = ULONG, S('X') = ULONG, S('b') = ULONG, S('B') = ULONG,
        S('n') = PLONG,

        S('c') = WINT, S('s') = PWCHAR,

        S('a') = DBL, S('e') = DBL, S('f') = DBL, S('g') = DBL,
        S('A') = DBL, S('E') = DBL, S('F') = DBL, S('G') = DBL,
    },
    [LLPRE] = {
        S('d') = LLONG, S('i') = LLONG, S('o') = ULLONG, S('u') = ULLONG,
        S('x') = ULLONG, S('X') = ULLONG, S('b') = ULLONG, S('B') = ULLONG,
        S('n') = PLLONG,
    },
    [JPRE] = {
        S('d') = INTMAX, S('i') = INTMAX, S('o') = UINTMAX, S('u') = UINTMAX,
        S('x') = UINTMAX, S('X') = UINTMAX, S('b') = UINTMAX, S('B') = UINTMAX,
        S('n') = PINTMAX,
    },
    [ZTPRE] = {
        S('d') = PTRDIFF, S('i') = PTRDIFF, S('o') = SIZE, S('u') = SIZE,
        S('x') = SIZE, S('X') = SIZE, S('b') = SIZE, S('B') = SIZE,
        S('n') = PPTRDIFF,
    },
    [CLPRE] = {
        S('a') = LDBL, S('e') = LDBL, S('f') = LDBL, S('g') = LDBL,
        S('A') = LDBL, S('E') = LDBL, S('F') = LDBL, S('G') = LDBL,
    },

    [WPRE] = {
        S('8') = W8PRE, S('1') = W1PRE, S('3') = W3PRE,
        S('6') = W6PRE, S('f') = WFPRE,
    },
    [W8PRE] = {
        S('d') = INT8, S('i') = INT8, S('o') = UINT8, S('u') = UINT8,
        S('x') = UINT8, S('X') = UINT8, S('b') = UINT8, S('B') = UINT8,
    },
    [W1PRE] = {
        S('6') = W16PRE,
    },
    [W3PRE] = {
        S('2') = W32PRE,
    },
    [W6PRE] = {
        S('4') = W64PRE,
    },
    [W16PRE] = {
        S('d') = INT16, S('i') = INT16, S('o') = UINT16, S('u') = UINT16,
        S('x') = UINT16, S('X') = UINT16, S('b') = UINT16, S('B') = UINT16,
    },
    [W32PRE] = {
        S('d') = INT32, S('i') = INT32, S('o') = UINT32, S('u') = UINT32,
        S('x') = UINT32, S('X') = UINT32, S('b') = UINT32, S('B') = UINT32,
    },
    [W64PRE] = {
        S('d') = INT64, S('i') = INT64, S('o') = UINT64, S('u') = UINT64,
        S('x') = UINT64, S('X') = UINT64, S('b') = UINT64, S('B') = UINT64,
    },

    [WFPRE] = {
        S('1') = WF1PRE, S('3') = WF3PRE, S('6') = WF6PRE, S('8') = WF8PRE,
    },
    [WF8PRE] = {
        S('d') = INT_FAST8, S('i') = INT_FAST8, S('o') = UINT_FAST8,
        S('u') = UINT_FAST8, S('x') = UINT_FAST8, S('X') = UINT_FAST8,
        S('b') = UINT_FAST8, S('B') = UINT_FAST8,
    },
    [WF1PRE] = {
        S('6') = WF16PRE,
    },
    [WF3PRE] = {
        S('2') = WF32PRE,
    },
    [WF6PRE] = {
        S('4') = WF64PRE,
    },
    [WF16PRE] = {
        S('d') = INT_FAST16, S('i') = INT_FAST16, S('o') = UINT_FAST16,
        S('u') = UINT_FAST16, S('x') = UINT_FAST16, S('X') = UINT_FAST16,
        S('b') = UINT_FAST16, S('B') = UINT_FAST16,
    },
    [WF32PRE] = {
        S('d') = INT_FAST32, S('i') = INT_FAST32, S('o') = UINT_FAST32,
        S('u') = UINT_FAST32, S('x') = UINT_FAST32, S('X') = UINT_FAST32,
        S('b') = UINT_FAST32, S('B') = UINT_FAST32,
    },
    [WF64PRE] = {
        S('d') = INT_FAST64, S('i') = INT_FAST64, S('o') = UINT_FAST64,
        S('u') = UINT_FAST64, S('x') = UINT_FAST64, S('X') = UINT_FAST64,
        S('b') = UINT_FAST64, S('B') = UINT_FAST64,
    },

};

static union arg pop_arg(va_list *ap, int type)
{
    union arg rv;
    switch (type)
    {
        case SCHAR:         rv.i = (signed char)va_arg(*ap, int); break;
        case SHORT:         rv.i = (short)va_arg(*ap, int); break;
        case INT:           rv.i = va_arg(*ap, int); break;
        case LONG:          rv.i = va_arg(*ap, long); break;
        case LLONG:         rv.i = va_arg(*ap, long long); break;
        case PTRDIFF:       rv.i = va_arg(*ap, ptrdiff_t); break;
        case WINT:          rv.i = va_arg(*ap, wint_t); break;
        case INT8:          rv.i = (int8_t)va_arg(*ap, int); break;
        case INT16:         rv.i = (int16_t)va_arg(*ap, int); break;
        case INT32:         rv.i = va_arg(*ap, int32_t); break;
        case INT64:         rv.i = va_arg(*ap, int64_t); break;
        case INT_FAST8:     rv.i = (int_fast8_t)va_arg(*ap, int); break;
        case INT_FAST32:    rv.i = va_arg(*ap, int_fast32_t); break;
        case INT_FAST64:    rv.i = va_arg(*ap, int_fast64_t); break;
        case INTMAX:        rv.i = va_arg(*ap, intmax_t); break;
        case UCHAR:         rv.u = (unsigned char)va_arg(*ap, int); break;
        case USHORT:        rv.u = (unsigned short)va_arg(*ap, int); break;
        case UINT:          rv.u = va_arg(*ap, unsigned int); break;
        case ULONG:         rv.u = va_arg(*ap, unsigned long); break;
        case ULLONG:        rv.u = va_arg(*ap, unsigned long long); break;
        case SIZE:          rv.u = va_arg(*ap, size_t); break;
        case UINT8:         rv.u = (uint8_t)va_arg(*ap, int); break;
        case UINT16:        rv.u = (uint16_t)va_arg(*ap, int); break;
        case UINT32:        rv.u = va_arg(*ap, uint32_t); break;
        case UINT64:        rv.u = va_arg(*ap, uint64_t); break;
        case UINT_FAST8:    rv.u = (uint_fast8_t)va_arg(*ap, int); break;
        case UINT_FAST32:   rv.u = va_arg(*ap, uint_fast32_t); break;
        case UINT_FAST64:   rv.u = va_arg(*ap, uint_fast64_t); break;
        case UINTMAX:       rv.u = va_arg(*ap, uintmax_t); break;

        case INT_FAST16:    if (sizeof (int_fast16_t) < sizeof (int))
                                rv.i = (int_fast16_t)va_arg(*ap, int);
                            else
                                rv.i = va_arg(*ap, int_fast16_t);
                            break;
        case UINT_FAST16:   if (sizeof (uint_fast16_t) < sizeof (int))
                                rv.u = (uint_fast16_t)va_arg(*ap, int);
                            else
                                rv.u = va_arg(*ap, uint_fast16_t);
                            break;

        case DBL:   rv.f = va_arg(*ap, double); break;
        case LDBL:  rv.f = va_arg(*ap, long double); break;

        case PSCHAR: case PSHORT: case PINT: case PLONG: case PLLONG: case PPTRDIFF: case PWCHAR: case PCHAR:
        case PVOID: case PINTMAX:
            rv.p = va_arg(*ap, void *); break;
    }
    return rv;
}

static int printf_core(FILE *restrict f, const char *restrict fmt, va_list *ap, int *nl_type, union arg *nl_arg)
{
    int rv = 0;
    int i18n = 0;
    char buf[MAX(8 * sizeof (uintmax_t), MB_LEN_MAX)];
    while (*fmt) {
        const char *a, *z;
        a = fmt + __stridx(fmt, '%');
        for (z = a; z[0] == '%' && z[1] == '%'; a++, z += 2);
        if (addo(&rv, a - fmt)) goto overflow;
        out(f, fmt, a - fmt);
        fmt = z;
        if (*fmt != '%') continue;

        fmt++;
        int c = *fmt++;
        int argpos = -1;
        int flags = 0;
        int width = 0;
        int prec = -1;
        int st = 0;
        if (c - '1' < 9u && *fmt == '$') {
            argpos = c - '1';
            c = fmt[1];
            fmt += 2;
        }
        while (c - ' ' < 32u && (FLG(c) & ALL_FLAGS)) {
            flags |= FLG(c);
            c = *fmt++;
        }

        if ((flags & FLG_ZERO) && (flags & FLG_LEFT))
            flags -= FLG_ZERO;

        if (c - '0' < 10u) {
            fmt--;
            width = getint(&fmt);
            c = *fmt++;
        } else if (c == '*') {
            if (*fmt - '1' < 9u && fmt[1] == '$') {
                if (i18n < 0) goto inval;
                i18n = 1;
                int t = fmt[0] - '1';
                if (!f) nl_type[t] = INT;
                else width = nl_arg[t].i;
                c = fmt[2];
                fmt += 3;
            } else {
                if (i18n > 0) goto inval;
                i18n = -1;
                if (f) width = va_arg(*ap, int);
                c = *fmt++;
            }
        }
        if (c == '.') {
            if (*fmt == '*') {
                if (fmt[1] - '1' < 9u && fmt[2] == '$') {
                    if (i18n < 0) goto inval;
                    i18n = 1;
                    int t = fmt[1] - '1';
                    if (!f) nl_type[t] = INT;
                    else prec = nl_arg[t].i;
                    c = fmt[3];
                    fmt += 4;
                } else {
                    if (i18n > 0) goto inval;
                    i18n = -1;
                    if (f) prec = va_arg(*ap, int);
                    c = fmt[1];
                    fmt += 2;
                }
            } else {
                prec = getint(&fmt);
                c = *fmt++;
            }
        }

        while (c - '0' < 'z' - '0' + 1u) {
            st = state_machine[st][c - '0'];
            if (!st) goto inval;
            if (st > END) break;
            c = *fmt++;
        }
        if (st < END) goto inval;

        union arg arg;
        if (st != NOTHING) {
            if ((argpos != -1 && i18n < 0)
                    || (argpos == -1 && i18n > 0))
                goto inval;
            if (argpos != -1) {
                i18n = 1;
                if (!f) nl_type[argpos] = st;
                else arg = nl_arg[argpos];
            } else if (f) arg = pop_arg(ap, st);
        }

        if (!f) {
            if (addo(&rv, MAX(width, prec))) goto overflow;
            continue;
        }

        const char *prefix = "";
        int preflen = 0;

        switch (c) {
            case 'm':
                arg.p = strerror(errno);
                /* fall through */
            case 'S':
            case 's':
                if (st == PWCHAR) {
                    const wchar_t *ws = arg.p? arg.p : L"(null)";
                    mbstate_t state = {0};
                    size_t mbslen = 0;
                    size_t n = 0;
                    for (; ws[n]; n++) {
                        size_t mbl = wcrtomb(buf, ws[n], &state);
                        if (mbl == -1) goto ilseq;
                        if (prec >= 0 && mbslen + mbl > prec) break;
                        mbslen += mbl;
                    }
                    if (addo(&rv, MAX(width, mbslen))) goto overflow;
                    pad(f, ' ', width, mbslen, flags);
                    memset(&state, 0, sizeof state);
                    for (size_t i = 0; i < n; i++) {
                        size_t mbl = wcrtomb(buf, ws[i], &state);
                        out(f, buf, mbl);
                    }
                    pad(f, ' ', width, mbslen, flags ^ FLG_LEFT);
                    continue;
                }
                a = arg.p? arg.p : "(null)";
                if (prec < 0) z = a + strlen(a);
                else {
                    z = memchr(a, 0, prec);
                    if (!z) z = a + prec;
                }
                prec = 0;
                break;

            case 'c':
            case 'C':
                prec = 0;
                if (st == WINT) {
                    mbstate_t state = {0};
                    size_t mblen = wcrtomb(buf, arg.i, &state);
                    if (mblen == -1) goto ilseq;
                    a = buf;
                    z = buf + mblen;
                } else {
                    a = buf;
                    buf[0] = arg.i;
                    z = buf + 1;
                }
                break;

            case 'B':
            case 'b':
                if (flags & FLG_ALT) {
                    prefix = "0B0b";
                    preflen = 2;
                    if (c & 32) prefix += 2;
                }
                z = buf + sizeof buf;
                a = fmt_b(buf + sizeof buf, arg.u);
                break;

            case 'd':
            case 'i':
                prefix = "-+ ";
                preflen = 1;
                if (arg.i < 0)
                    arg.u = -arg.i;
                else if (flags & FLG_SIGN)
                    prefix++;
                else if (flags & FLG_MARK)
                    prefix += 2;
                else
                    preflen = 0;
                /* fall through */
            case 'u':
                z = buf + sizeof buf;
                a = fmt_d(buf + sizeof buf, arg.u);
                break;

            case 'o':
                z = buf + sizeof buf;
                a = fmt_o(buf + sizeof buf, arg.u);
                break;

            case 'p':
                arg.u = (uintptr_t)arg.p;
                flags |= FLG_ALT;
                if (prec < 2 * (int)sizeof (long)) prec = 2 * sizeof (long);
                /* fall through */
            case 'x':
            case 'X':
                if (flags & FLG_ALT) {
                    prefix = "0X0x";
                    preflen = 2;
                    if (c & 32) prefix += 2;
                }
                z = buf + sizeof buf;
                a = fmt_x(buf + sizeof buf, arg.u, c & 32);
                break;

            case 'n':
                switch (st) {
                    case PSCHAR:    *(signed char *)arg.p = rv; break;
                    case PSHORT:    *(short *)arg.p = rv; break;
                    case PINT:      *(int *)arg.p = rv; break;
                    case PLONG:     *(long *)arg.p = rv; break;
                    case PLLONG:    *(long long *)arg.p = rv; break;
                    case PPTRDIFF:  *(ptrdiff_t *)arg.p = rv; break;
                    case PINTMAX:   *(intmax_t *)arg.p = rv; break;
                }
                continue;

            case 'A': case 'E': case 'F': case 'G':
            case 'a': case 'e': case 'f': case 'g':
                if (addo(&rv, __fmt_fp(f, arg.f, width, prec, flags, c, st == LDBL? fmt_ldouble : fmt_double)))
                    goto overflow;
                continue;
        }

        if (prec < 0) prec = 1;
        if (z - a > INT_MAX) goto overflow;
        if (prec < z - a) prec = z - a;
        if (c == 'o' && (flags & FLG_ALT) && prec == z - a) prec++;
        if (prec > INT_MAX - preflen) goto overflow;
        if (addo(&rv, MAX(width, prec+preflen))) goto overflow;
        pad(f, ' ', width, prec+preflen, flags);
        out(f, prefix, preflen);
        pad(f, '0', width, prec+preflen, flags ^ FLG_ZERO);
        pad(f, '0', prec, z - a, 0);
        out(f, a, z - a);
        pad(f, ' ', width, prec+preflen, flags ^ FLG_LEFT);
    }
    if (!f && i18n > 0) {
        size_t i;
        for (i = 0; nl_type[i]; i++)
            nl_arg[i] = pop_arg(ap, nl_type[i]);
        for (; i < NL_ARGMAX; i++)
            if (nl_type[i])
                goto inval;
    }
    return rv;

ilseq:
    errno = EILSEQ;
    return -1;
overflow:
    errno = EOVERFLOW;
    return -1;
inval:
    errno = EINVAL;
    return -1;
}

int vfprintf(FILE *restrict f, const char *restrict fmt, va_list ap)
{
    int nl_type[NL_ARGMAX+1] = {0};
    union arg nl_arg[NL_ARGMAX];
    int ret;
    va_list ap2;
    __FLOCK(f);
    va_copy(ap2, ap);
    ret = __towrite(f);
    if (!ret)
        ret = printf_core(0, fmt, &ap2, nl_type, nl_arg);
    if (ret != -1) {
        unsigned char tmpbuf[80];
        unsigned char *oldbuf;
        if (!f->buf_size) {
            oldbuf = f->buf;
            f->buf = tmpbuf;
            f->buf_size = sizeof tmpbuf;
            f->dir = 0;
            __towrite(f);
        }
        ret = printf_core(f, fmt, &ap2, nl_type, nl_arg);
        if (f->buf == tmpbuf) {
            if (fflush(f)) ret = -1;
            f->buf = oldbuf;
            f->buf_size = 0;
        }
    }
    va_end(ap2);
    __FUNLOCK(f);
    return ret;
}
