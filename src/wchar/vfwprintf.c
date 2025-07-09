#include <wchar.h>
#include <stdio.h>
#include <stdint.h>
#include <wctype.h>
#include <stdarg.h>
#include <stddef.h>
#include <limits.h>
#include "libc.h"
#include <errno.h>
#include <string.h>

static int addo(int *x, size_t add)
{
    if (add > INT_MAX - *x) return 1;
    *x += add;
    return 0;
}

static int getint(const wchar_t **ps)
{
    int rv = 0;
    const wchar_t *s = *ps;
    while (iswdigit(*s)) {
        rv = 10 * rv + *s - L'0';
        s++;
    }
    *ps = s;
    return rv;
}

#define FLG(x) (1 << ((x) - ' '))
#define FLG_THOU FLG('\'')
#define FLG_LEFT FLG('-')
#define FLG_SIGN FLG('+')
#define FLG_MARK FLG(' ')
#define FLG_ALT FLG('#')
#define FLG_ZERO FLG('0')
#define ALL_FLAGS (FLG_THOU | FLG_LEFT | FLG_SIGN | FLG_MARK | FLG_ALT | FLG_ZERO)

static char *print_flags(char *p, int flags)
{
    if (flags & FLG_THOU)
        *p++ = '\'';
    if (flags & FLG_LEFT)
        *p++ = '-';
    if (flags & FLG_SIGN)
        *p++ = '+';
    if (flags & FLG_MARK)
        *p++ = ' ';
    if (flags & FLG_ALT)
        *p++ = '#';
    if (flags & FLG_ZERO)
        *p++ = '0';
    return p;
}

static void prepare_format(char *p, int flags, int extra, int c)
{
    p[0] = '%';
    p = print_flags(p + 1, flags);
    *p++ = '*';
    *p++ = '.';
    *p++ = '*';
    if (extra) *p++ = extra;
    *p++ = c;
    *p = 0;
}

static void out(FILE *f, const wchar_t *ws, size_t len)
{
    if (f) while (len--) fputwc(*ws++, f);
}
static void pad(FILE *f, int width, int len, int flags)
{
    if (len >= width || (flags & FLG_LEFT)) return;
    width -= len;
    while (width--) fputwc(' ', f);
}

union arg {
    uintmax_t u;
    intmax_t i;
    long double f;
    void *p;
};

enum {
    BARE, HPRE, HHPRE, LPRE, LLPRE, JPRE, ZTPRE, CLPRE,
    WPRE, W1PRE, W3PRE, W6PRE, W8PRE, W16PRE, W32PRE, W64PRE,
    WFPRE, WF1PRE, WF3PRE, WF6PRE, WF8PRE, WF16PRE, WF32PRE, WF64PRE,
    END,
    NOTHING,
    SCHAR, SHORT, INT, LONG, LLONG, INTMAX, PTRDIFF, INT8, INT16, INT32, INT64,
    INT_FAST8, INT_FAST16, INT_FAST32, INT_FAST64, WINT,
    UCHAR, USHORT, UINT, ULONG, ULLONG, UINTMAX, SIZE, UINT8, UINT16, UINT32,
    UINT64, UINT_FAST8, UINT_FAST16, UINT_FAST32, UINT_FAST64,
    PSCHAR, PSHORT, PINT, PLONG, PLLONG, PINTMAX, PPTRDIFF, PCHAR, PWCHAR,
    PVOID, PINT8, PINT16, PINT32, PINT64, PINT_FAST8, PINT_FAST16,
    PINT_FAST32, PINT_FAST64,
    DBL, LDBL
};

static union arg pop_arg(int type, va_list *ap)
{
    union arg arg;
    switch (type) {
        case SCHAR:             arg.i = (signed char)va_arg(*ap, int); break;
        case SHORT:             arg.i = (short)va_arg(*ap, int); break;
        case INT:               arg.i = va_arg(*ap, int); break;
        case LONG:              arg.i = va_arg(*ap, long); break;
        case LLONG:             arg.i = va_arg(*ap, long long); break;
        case INTMAX:            arg.i = va_arg(*ap, intmax_t); break;
        case PTRDIFF:           arg.i = va_arg(*ap, ptrdiff_t); break;
        case INT8:              arg.i = (int8_t)va_arg(*ap, int); break;
        case INT16:             arg.i = (int16_t)va_arg(*ap, int); break;
        case INT32:             arg.i = va_arg(*ap, int32_t); break;
        case INT64:             arg.i = va_arg(*ap, int64_t); break;
        case INT_FAST8:         arg.i = (int_fast8_t)va_arg(*ap, int); break;
        case INT_FAST32:        arg.i = va_arg(*ap, int_fast32_t); break;
        case INT_FAST64:        arg.i = va_arg(*ap, int_fast64_t); break;
        case WINT:              arg.i = va_arg(*ap, wint_t); break;

        case UCHAR:             arg.u = (unsigned char)va_arg(*ap, int); break;
        case USHORT:            arg.u = (unsigned short)va_arg(*ap, int); break;
        case UINT:              arg.u = va_arg(*ap, unsigned int); break;
        case ULONG:             arg.u = va_arg(*ap, unsigned long); break;
        case ULLONG:            arg.u = va_arg(*ap, unsigned long long); break;
        case UINTMAX:           arg.u = va_arg(*ap, uintmax_t); break;
        case SIZE:              arg.u = va_arg(*ap, size_t); break;
        case UINT8:             arg.u = (uint8_t)va_arg(*ap, int); break;
        case UINT16:            arg.u = (uint16_t)va_arg(*ap, int); break;
        case UINT32:            arg.u = va_arg(*ap, uint32_t); break;
        case UINT64:            arg.u = va_arg(*ap, uint64_t); break;
        case UINT_FAST8:        arg.u = (uint_fast8_t)va_arg(*ap, int); break;
        case UINT_FAST32:       arg.u = va_arg(*ap, uint_fast32_t); break;
        case UINT_FAST64:       arg.u = va_arg(*ap, uint_fast64_t); break;

        case INT_FAST16:        if (sizeof (int_fast16_t) < sizeof (int))
                                    arg.i = (int_fast16_t)va_arg(*ap, int);
                                else
                                    arg.i = va_arg(*ap, int_fast16_t);
                                break;

        case UINT_FAST16:       if (sizeof (uint_fast16_t) < sizeof (int))
                                    arg.u = (uint_fast16_t)va_arg(*ap, int);
                                else
                                    arg.u = va_arg(*ap, uint_fast16_t);
                                break;

        case PSCHAR: case PSHORT: case PINT: case PLONG: case PLLONG:
        case PINTMAX: case PPTRDIFF: case PCHAR: case PWCHAR: case PVOID:
        case PINT8: case PINT16: case PINT32: case PINT64:
        case PINT_FAST8: case PINT_FAST16: case PINT_FAST32: case PINT_FAST64:
                                arg.p = va_arg(*ap, void *);
                                break;

        case DBL:               arg.f = va_arg(*ap, double); break;
        case LDBL:              arg.f = va_arg(*ap, long double); break;
    }
    return arg;
}

#define S(x) [(x) - '0']
static const unsigned char state_machine[END]['z' - '0' + 1] = {
    [BARE] = {
        S('h') = HPRE, S('l') = LPRE, S('L') = CLPRE, S('j') = JPRE,
        S('w') = WPRE, S('t') = ZTPRE, S('z') = ZTPRE,

        S('d') = INT, S('i') = INT, S('b') = UINT, S('B') = UINT, S('o') = UINT,
        S('u') = UINT, S('x') = UINT, S('X') = UINT,

        S('a') = DBL, S('e') = DBL, S('f') = DBL, S('g') = DBL,
        S('A') = DBL, S('E') = DBL, S('F') = DBL, S('G') = DBL,

        S('c') = INT, S('s') = PCHAR, S('C') = WINT, S('S') = PWCHAR,
        S('p') = PVOID, S('n') = PINT, S('m') = NOTHING,
    },
    [HPRE] = {
        S('h') = HHPRE,

        S('d') = SHORT, S('i') = SHORT, S('b') = USHORT, S('B') = USHORT,
        S('o') = USHORT, S('u') = USHORT, S('x') = USHORT, S('X') = USHORT,
        S('n') = PSHORT,
    },
    [HHPRE] = {
        S('d') = SCHAR, S('i') = SCHAR, S('b') = UCHAR, S('B') = UCHAR,
        S('o') = UCHAR, S('u') = UCHAR, S('x') = UCHAR, S('X') = UCHAR,
        S('n') = PSCHAR,
    },
    [LPRE] = {
        S('l') = LLPRE,

        S('d') = LONG, S('i') = LONG, S('b') = ULONG, S('B') = ULONG,
        S('o') = ULONG, S('u') = ULONG, S('x') = ULONG, S('X') = ULONG,
        S('n') = PLONG,

        S('a') = DBL, S('e') = DBL, S('f') = DBL, S('g') = DBL,
        S('A') = DBL, S('E') = DBL, S('F') = DBL, S('G') = DBL,

        S('c') = WINT, S('s') = PWCHAR,
    },
    [LLPRE] = {
        S('d') = LLONG, S('i') = LLONG, S('b') = ULLONG, S('B') = ULLONG,
        S('o') = ULLONG, S('u') = ULLONG, S('x') = ULLONG, S('X') = ULLONG,
        S('n') = PLLONG,
    },
    [JPRE] = {
        S('d') = INTMAX, S('i') = INTMAX, S('b') = UINTMAX, S('B') = UINTMAX,
        S('o') = UINTMAX, S('u') = UINTMAX, S('x') = UINTMAX, S('X') = UINTMAX,
        S('n') = PINTMAX,
    },
    [ZTPRE] = {
        S('d') = PTRDIFF, S('i') = PTRDIFF, S('n') = PPTRDIFF, S('b') = SIZE,
        S('B') = SIZE, S('o') = SIZE, S('u') = SIZE, S('x') = SIZE,
        S('X') = SIZE, S('n') = PPTRDIFF,
    },
    [CLPRE] = {
        S('a') = LDBL, S('e') = LDBL, S('f') = LDBL, S('g') = LDBL,
        S('A') = LDBL, S('E') = LDBL, S('F') = LDBL, S('G') = LDBL,
    },
    [WPRE] = {
        S('f') = WFPRE, S('1') = W1PRE, S('3') = W3PRE, S('6') = W6PRE,
        S('8') = W8PRE,
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
    [W8PRE] = {
        S('d') = INT8, S('i') = INT8, S('b') = UINT8, S('B') = UINT8,
        S('o') = UINT8, S('u') = UINT8, S('x') = UINT8, S('X') = UINT8,
        S('n') = PINT8,
    },
    [W16PRE] = {
        S('d') = INT16, S('i') = INT16, S('b') = UINT16, S('B') = UINT16,
        S('o') = UINT16, S('u') = UINT16, S('x') = UINT16, S('X') = UINT16,
        S('n') = PINT16,
    },
    [W32PRE] = {
        S('d') = INT32, S('i') = INT32, S('b') = UINT32, S('B') = UINT32,
        S('o') = UINT32, S('u') = UINT32, S('x') = UINT32, S('X') = UINT32,
        S('n') = PINT32,
    },
    [W64PRE] = {
        S('d') = INT64, S('i') = INT64, S('b') = UINT64, S('B') = UINT64,
        S('o') = UINT64, S('u') = UINT64, S('x') = UINT64, S('X') = UINT64,
        S('n') = PINT64,
    },
    [WFPRE] = {
        S('1') = WF1PRE, S('3') = WF3PRE, S('6') = WF6PRE, S('8') = WF8PRE,
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
    [WF8PRE] = {
        S('d') = INT_FAST8, S('i') = INT_FAST8, S('b') = UINT_FAST8,
        S('B') = UINT_FAST8, S('o') = UINT_FAST8, S('u') = UINT_FAST8,
        S('x') = UINT_FAST8, S('X') = UINT_FAST8, S('n') = PINT_FAST8,
    },
    [WF16PRE] = {
        S('d') = INT_FAST16, S('i') = INT_FAST16, S('b') = UINT_FAST16,
        S('B') = UINT_FAST16, S('o') = UINT_FAST16, S('u') = UINT_FAST16,
        S('x') = UINT_FAST16, S('X') = UINT_FAST16, S('n') = PINT_FAST16,
    },
    [WF32PRE] = {
        S('d') = INT_FAST32, S('i') = INT_FAST32, S('b') = UINT_FAST32,
        S('B') = UINT_FAST32, S('o') = UINT_FAST32, S('u') = UINT_FAST32,
        S('x') = UINT_FAST32, S('X') = UINT_FAST32, S('n') = PINT_FAST32,
    },
    [WF64PRE] = {
        S('d') = INT_FAST64, S('i') = INT_FAST64, S('b') = UINT_FAST64,
        S('B') = UINT_FAST64, S('o') = UINT_FAST64, S('u') = UINT_FAST64,
        S('x') = UINT_FAST64, S('X') = UINT_FAST64, S('n') = PINT_FAST64,
    },
};

static int wprintf_core(FILE *f, const wchar_t *fmt, va_list *ap, int *nl_type, union arg *nl_arg)
{
    int i18n = 0;
    int rv = 0;
    char fmtbuf[sizeof "%*.*jd" + 6];

    while (*fmt) {
        const wchar_t *a = fmt + __wcsidx(fmt, L'%');
        const wchar_t *z;
        for (z = a; z[0] == L'%' && z[1] == L'%'; a++, z += 2);
        if (addo(&rv, a - fmt)) goto overflow;
        out(f, fmt, a - fmt);
        fmt = z;
        if (*fmt != '%') continue;

        fmt++;
        wint_t c = *fmt++;
        int argpos = -1;
        int flags = 0;
        int width = 0;
        int prec = -1;
        int state = 0;
        if (c - L'1' < 9u && *fmt == L'$') {
            argpos = c - L'1';
            c = fmt[1];
            fmt += 2;
        }
        while (c - L' ' < 32u && (FLG(c) & ALL_FLAGS)) {
            flags |= FLG(c);
            c = *fmt++;
        }
        if (c - '0' < 10u) {
            fmt--;
            width = getint(&fmt);
            c = *fmt++;
        } else if (c == L'*') {
            if (*fmt - L'1' < 9u && fmt[1] == L'$') {
                if (i18n < 0) goto inval;
                i18n = 1;
                int t = *fmt - L'1';
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
                if (fmt[1] - L'1' < 9u && fmt[2] == L'$') {
                    if (i18n < 0) goto inval;
                    i18n = 1;
                    int t = fmt[1] - L'1';
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

        for (;;) {
            if (c - L'0' > (unsigned)(L'z' - L'0')) goto inval;
            state = state_machine[state][c - '0'];
            if (!state) goto inval;
            if (state > END) break;
            c = *fmt++;
        }
        if (state != NOTHING) {
            if (i18n < 0 && argpos >= 0) goto inval;
            if (i18n > 0 && argpos < 0) goto inval;
            if (argpos < 0) i18n = -1;
            else i18n = 1;
        }

        if (!f) continue;

        union arg arg;
        if (argpos >= 0) arg = nl_arg[argpos];
        else arg = pop_arg(state, ap);

        int printlen;
        wint_t wcbuf;
        switch (c) {
            /* XXX: I am assuming uintmax_t is passed like intmax_t here */
            case 'b': case 'B': case 'd': case 'i':
            case 'o': case 'u': case 'x': case 'X':
                prepare_format(fmtbuf, flags, 'j', c);
                printlen = fprintf(f, fmtbuf, width, prec, arg.u);
                break;

            case 'a': case 'e': case 'f': case 'g':
            case 'A': case 'E': case 'F': case 'G':
                prepare_format(fmtbuf, flags, 'L', c);
                printlen = fprintf(f, fmtbuf, width, prec, arg.f);
                break;

            case 'p':
                prepare_format(fmtbuf, flags, 0, 'p');
                printlen = fprintf(f, fmtbuf, width, prec, arg.p);
                break;

            case 'n':
                switch (state) {
                    case PSCHAR:        *(signed char *)arg.p = rv; break;
                    case PSHORT:        *(short *)arg.p = rv; break;
                    case PINT:          *(int *)arg.p = rv; break;
                    case PLONG:         *(long *)arg.p = rv; break;
                    case PLLONG:        *(long long *)arg.p = rv; break;
                    case PINTMAX:       *(intmax_t *)arg.p = rv; break;
                    case PPTRDIFF:      *(ptrdiff_t *)arg.p = rv; break;
                    case PINT8:         *(int8_t *)arg.p = rv; break;
                    case PINT16:        *(int16_t *)arg.p = rv; break;
                    case PINT32:        *(int32_t *)arg.p = rv; break;
                    case PINT64:        *(int64_t *)arg.p = rv; break;
                    case PINT_FAST8:    *(int_fast8_t *)arg.p = rv; break;
                    case PINT_FAST16:   *(int_fast16_t *)arg.p = rv; break;
                    case PINT_FAST32:   *(int_fast32_t *)arg.p = rv; break;
                    case PINT_FAST64:   *(int_fast64_t *)arg.p = rv; break;
                }
                continue;

            case 'C':
            case 'c':
                wcbuf = state == WINT? arg.i : btowc(arg.i);
                if (wcbuf == WEOF) goto ilseq;
                if (addo(&rv, MAX(width, 1))) goto overflow;
                pad(f, width, 1, flags);
                out(f, &(wchar_t){wcbuf}, 1);
                pad(f, width, 1, flags ^ FLG_LEFT);
                continue;

            case 'm':
                arg.p = strerror(errno);
                /* fall through */
            case 's':
            case 'S':
                if (state == PWCHAR) {
                    const wchar_t *p = arg.p;
                    const wchar_t *end;
                    if (prec < 0)
                        end = p + wcslen(p);
                    else {
                        end = wmemchr(p, 0, prec);
                        if (!end) end = p + prec;
                    }
                    if (addo(&rv, MAX(width, end - p))) goto overflow;
                    pad(f, width, end - p, flags);
                    out(f, p, end - p);
                    pad(f, width, end - p, flags ^ FLG_LEFT);
                } else {
                    const char *p = arg.p;
                    wchar_t buf;
                    mbstate_t mbs = {0};
                    size_t wslen = 0;
                    for (;;) {
                        size_t mbclen = mbrtowc(&buf, p, -1, &mbs);
                        /* mblen == -2 is impossible since I set n > 4 */
                        if (mbclen == -1) return -1;
                        if (mbclen == 0) break;
                        p += mbclen;
                        wslen++;
                        if (prec > 0 && (size_t)prec == wslen) break;
                    }

                    p = arg.p;
                    memset(&mbs, 0, sizeof mbs);

                    if (addo(&rv, MAX(wslen, width))) goto overflow;
                    pad(f, width, wslen, flags);
                    for (size_t i = 0; i < wslen; i++) {
                        size_t mbclen = mbrtowc(&buf, p, -1, &mbs);
                        out(f, &buf, 1);
                        p += mbclen;
                    }
                    pad(f, width, wslen, flags ^ FLG_LEFT);
                }
                continue;
        }
        if (printlen < 0) return -1;
        if (addo(&rv, printlen)) goto overflow;
    }
    if (!f && i18n > 0) {
        size_t i;
        for (i = 0; nl_type[i]; i++)
            nl_arg[i] = pop_arg(nl_type[i], ap);
        for (; i < NL_ARGMAX; i++)
            if (nl_type[i])
                goto inval;
    }

    return rv;

inval:
    errno = EINVAL;
    return -1;
overflow:
    errno = EOVERFLOW;
    return -1;
ilseq:
    errno = EILSEQ;
    return -1;
}

int vfwprintf(FILE *restrict f, const wchar_t *restrict fmt, va_list ap)
{
    int nl_type[NL_ARGMAX+1] = {0};
    union arg nl_arg[NL_ARGMAX];
    va_list ap2;
    __FLOCK(f);
    va_copy(ap2, ap);
    int rv = wprintf_core(0, fmt, &ap2, nl_type, nl_arg);
    if (rv >= 0) {
        unsigned char *orig_buf = 0;
        unsigned char tempbuf[80];
        if (!f->buf_size) {
            orig_buf = f->buf;
            f->buf = tempbuf;
            f->buf_size = sizeof tempbuf;
        }
        rv = wprintf_core(f, fmt, &ap2, nl_type, nl_arg);
        if (orig_buf) {
            if (fflush(f))
                rv = -1;
            f->buf = orig_buf;
            f->buf_size = 0;
        }
    }
    __FUNLOCK(f);
    return rv;
}
