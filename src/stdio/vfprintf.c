#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <wchar.h>
#include <float.h>
#include <math.h>
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

/* Serendipity: All flags characters are within 32 code points of the space */
#define FLG(x)  (1ul << ((x) - ' '))
#define FLG_THOU    FLG('\'')
#define FLG_LEFT    FLG('-')
#define FLG_SIGN    FLG('+')
#define FLG_MARK    FLG(' ')
#define FLG_ZERO    FLG('0')
#define FLG_ALT     FLG('#')
#define ALL_FLAGS (FLG_THOU | FLG_LEFT | FLG_SIGN | FLG_MARK | FLG_ZERO | FLG_ALT)

static void pad(FILE *f, int c, int w, int len, int flags)
{
    char buf[256];
    if (len >= w || (flags & (FLG_LEFT | FLG_ZERO))) return;
    len = w - len;
    memset(buf, c, len);
    while (len > sizeof buf)
        out(f, buf, sizeof buf), len -= sizeof buf;
    out(f, buf, len);
}

static size_t fmt_hexfloat(FILE *f, long double x, int width, int prec, int flags,
        int c, const char *prefix, int preflen)
{
    if (c & 32) prefix += 9;
    uint32_t big[(LDBL_MANT_DIG + 31) / 32 + 1];
    uint32_t *a = big;
    uint32_t *z = big;
    int e2;
    x = frexpl(x, &e2);
    if (x) {
        x *= 2;
        e2--;
    }
    do {
        *z = x;
        x = 0x1p32 * (x - *z++);
    } while (x);

    if (prec < 0) prec = (LDBL_MANT_DIG + 3) / 4;
    if (prec < 8 * (z - a)) {
        uint32_t *r = a + 1 + prec/8;
        uint32_t mask = (1ul << (4 * (8 - prec % 8) - 1) << 1) - 1;
        uint32_t away = *r & mask;
        *r -= away;
        long double large = 1/LDBL_EPSILON;
        if ((*r & (mask << 1)) || (mask == UINT32_MAX && r > a && (r[-1] & 1)))
            large += 1.0;
        long double small;
        if (away == 0 && z == r + 1)
            small = 0;
        else if (away <= mask/2)
            small = 0.25;
        else if (away == (mask/2) + 1 && z == r + 1)
            small = 0.5;
        else
            small = 0.75;
        if (*prefix == '-')
            large = -large, small = -small;
        if (large + small != large) {
            *r += mask + 1;
            while (r > a && *r == 0) {
                --r;
                ++*r;
            }
            if (r == a) {
                *a = 1;
                e2++;
            }
        }
        z = r + 1;
    }
    char ebuf[8];
    char digbuf[8];
    char *ez = ebuf + sizeof ebuf;
    char *ea = fmt_d(ez, abs(e2));
    if (ea == ez) *--ea = '0';
    if (e2 < 0) *--ea = '-';
    else *--ea = '+';
    *--ea = 'P' | (c & 32);
    /* unpadded output length is now:
     *      preflen for the prefix
     *  +   1 for the pre-radix digit
     *  +   1 for the radix point if it appears
     *  +   prec for the post-radix digits
     *  +   (ez - ea) for the exponent
     */
    int olen = preflen + 1 + (prec > 0 || (flags & FLG_ALT)) + (ez - ea);
    if (addo(&olen, prec)) return INT_MAX + 1ul;
    pad(f, ' ', width, olen, flags);
    out(f, prefix, preflen);
    pad(f, '0', width, olen, flags ^ FLG_ZERO);
    char *dz = digbuf + sizeof digbuf;
    char *da = fmt_x(dz, *a, c & 32);
    if (da == dz) *--da = '0';
    out(f, da, dz - da);
    if (prec > 0 || (flags & FLG_ALT))
        out(f, ".", 1);
    for (const uint32_t *i = a + 1; i < z && prec > 0; i++, prec -= 8)
    {
        da = fmt_x(dz, *i, c & 32);
        while (da > digbuf) *--da = '0';
        out(f, digbuf, MIN(prec, 8));
    }
    pad(f, '0', prec, 0, 0);
    out(f, ea, ez - ea);
    pad(f, ' ', width, olen, flags ^ FLG_LEFT);
    return MAX(olen, width);
}

static uint32_t pow10(int exp)
{
    uint32_t rv = 1;
    for (; exp--; rv *= 10);
    return rv;
}

static int countdig(uint32_t val)
{
    int rv = 1;
    for (uint32_t x = 10; x <= val; x *= 10, rv++);
    return rv;
}

#define BILLION 1000000000
static size_t fmt_decfloat(FILE *f, long double x, int width, int prec, int flags,
        int c, const char *prefix, int preflen, int isld)
{
    const int mant_dig = isld? LDBL_MANT_DIG : DBL_MANT_DIG;
    const int arrsz = isld?(LDBL_MANT_DIG - LDBL_MIN_EXP + 8) / 9 + 1 :
        (DBL_MANT_DIG - DBL_MIN_EXP + 8) / 9 + 1;
    uint32_t big[arrsz];
    int e2;
    x = frexpl(x, &e2);
    if (x) {
        x *= 0x1p29;
        e2 -= 29;
    }
    uint32_t *a = e2 < 0? big : big + arrsz - mant_dig;
    uint32_t *z, *rp;
    z = rp = a;
    do {
        *z = x;
        x = BILLION * (x - *z++);
    } while (x);

    while (e2 > 0) {
        int sh = MIN(29, e2);
        e2 -= sh;
        uint64_t carry = 0;
        for (uint32_t *i = z; i-- > a;) {
            carry |= (uint64_t)*i << sh;
            *i = carry % BILLION;
            carry /= BILLION;
        }
        if (carry) *--a = carry;
        while (z > a + 1 && !z[-1]) z--;
    }

    while (e2 < 0) {
        int sh = MIN(9, -e2);
        e2 += sh;
        uint32_t shiftmask = (1ul << sh) - 1;
        uint32_t carry_out;
        uint32_t carry_in = 0;
        for (uint32_t *i = a; i < z; i++) {
            carry_out = *i & shiftmask;
            *i = (BILLION >> sh) * carry_in + (*i>>sh);
            carry_in = carry_out;
        }
        if (carry_in) {
            *z++ = (BILLION >> sh) * carry_in;
        }
        while (a < z - 1 && !*a) a++;
    }

    int do_round = 0;
    uint32_t *r;
    uint32_t mod;
    if (prec < 0) prec = 6;
    if ((c|32) == 'f') {
        do_round = prec < 9 * (z - rp);
        if (do_round) {
            r = rp + 1 + prec / 9;
            mod = pow10(9 - (prec % 9));
        }
    } else {
        int num_dig_in_lead = countdig(*a);
        int num_dig_total = 9 * (z - a - 1) + num_dig_in_lead;
        do_round = prec < num_dig_total - ((c|32) == 'e');
        if (do_round) {
            int num_dig = prec + ((c|32) == 'e') - num_dig_in_lead;
            r = a + 1 + ((num_dig + 9) / 9 - 1);
            mod = pow10(9 - (num_dig + 9) % 9);
        }
    }

    if (do_round) {
        uint32_t away = *r % mod;
        *r -= away;
        long double large, small;
        large = 1/LDBL_EPSILON;
        if (((*r / mod) & 1)
                || (mod == BILLION && r > a && (r[-1] & 1)))
            large += 1;
        if (away == 0 && r == z - 1)
            small = 0;
        else if (away < mod / 2)
            small = 0.25;
        else if (away == mod / 2 && r == z - 1)
            small = 0.5;
        else
            small = 0.75;

        if (preflen && *prefix == '-')
            large = -large, small = -small;
        if (large + small != large) {
            *r += mod;
            while (r > a && *r == BILLION) {
                *r = 0;
                --r;
                ++*r;
            }
            if (*r == BILLION) {
                *r = 0;
                r[-1] = 1;
                a = r - 1;
            }
        }
        z = r + 1;
    }
    
    int e10;
    if ((c|32) != 'f') {
        int num_dig_in_lead = *a? countdig(*a) : 0;
        e10 = (rp - a) * 9 + num_dig_in_lead - 1;
    }
    int trim_zeroes = 0;
    if ((c|32) == 'g') {
        int p = prec;
        if (!p) p = 1;
        if (e10 >= -4 && e10 < p) {
            c--;
            prec = p - (e10 + 1);
        } else {
            c -= 2;
            prec = p - 1;
        }
        if (!(flags & FLG_ALT)) trim_zeroes = 1;
    }

    if ((c|32) == 'f') {
        if (a > rp) a = rp;
        if (z <= rp) z = rp + 1;
        if (trim_zeroes) {
            if (prec > (z - rp) * 9) prec = (z - rp) * 9;
            while (prec > 0 && (rp[1 + prec/9] / pow10(9 - prec % 9) % 10) == 0)
                prec--;
        }
        char digbuf[9];
        char *dz = digbuf + sizeof digbuf;
        char *da;
        int num_dig_in_lead = countdig(*a);
        /* total output length without padding:
         *      num_dig_in_lead + 9 * (rp - a)  for the pre-radix digits
         *  +   1                               for the radix point if it is printed
         *  +   prec                            for the post-radix digits
         */
        int outlen = num_dig_in_lead + 9 * (rp - a) + (prec > 0 || (flags & FLG_ALT));
        if (addo(&outlen, prec)) return INT_MAX + 1ul;
        pad(f, ' ', width, outlen, flags);
        out(f, prefix, preflen);
        pad(f, '0', width, outlen, flags ^ FLG_ZERO);
        da = fmt_d(dz, *a);
        if (da == dz) *--da = '0';
        out(f, da, dz - da);
        for (const uint32_t *i = a + 1; i <= rp; i++) {
            da = fmt_d(dz, *a);
            while (da > digbuf) *--da = '0';
            out(f, da, 9);
        }
        if (prec > 0 || (flags & FLG_ALT))
            out(f, ".", 1);
        for (const uint32_t *i = rp + 1; i < z && prec > 0; i++, prec -= 9) {
            da = fmt_d(dz, *i);
            while (da > digbuf) *--da = '0';
            out(f, da, MIN(prec, 9));
        }
        pad(f, '0', prec, 0, 0);
        pad(f, ' ', width, outlen, flags ^ FLG_LEFT);
        return MAX(width, outlen);
    } else {
        char ebuf[8];
        char *ez = ebuf + sizeof ebuf;
        char *ea = fmt_d(ez, abs(e10));
        if (ez == ea) *--ea = '0';
        if (ea == ez - 1) *--ea = '0';
        if (e10 < 0) *--ea = '-';
        else *--ea = '+';
        *--ea = c;
        char digbuf[9];
        char *dz = digbuf + sizeof digbuf;
        char *da = fmt_d(dz, *a);
        if (da == dz) *--da = '0';
        int num_dig_in_lead = dz - da;
        if (trim_zeroes) {
            if (prec > 9 * (z - a - 1) + num_dig_in_lead - 1)
                prec = 9 * (z - a - 1) + num_dig_in_lead - 1;
            while (prec >= num_dig_in_lead && a[1 + (prec - num_dig_in_lead)/9] / pow10(9 - (prec - num_dig_in_lead) % 9) % 10 == 0)
                prec--;
            if (prec < num_dig_in_lead) {
                while (prec > 0 && da[prec] == '0') prec--;
            }
        }
        /* total output length:
         *      1               for the pre-radix digit
         *  +   1               for the radix if it is printed
         *  +   prec            for the post-radix digits
         *  +   (ez - ea)       for the exponent part
         */
        int outlen = 1 + (prec > 0 || (flags & FLG_ALT)) + (ez - ea);
        if (addo(&outlen, prec)) return INT_MAX + 1ul;
        pad(f, ' ', width, outlen, flags);
        out(f, prefix, preflen);
        pad(f, '0', width, outlen, flags ^ FLG_ZERO);
        out(f, da, 1);
        if (prec > 0 || (flags & FLG_ALT))
            out(f, ".", 1);
        out(f, da + 1, MIN(dz - da - 1, prec));
        prec -= num_dig_in_lead - 1;
        for (const uint32_t *i = a + 1; i < z && prec > 0; i++, prec -= 9)
        {
            da = fmt_d(dz, *i);
            while (da > digbuf) *--da = '0';
            out(f, da, MIN(9, prec));
        }
        pad(f, '0', prec, 0, 0);
        out(f, ea, ez - ea);
        pad(f, ' ', width, outlen, flags ^ FLG_LEFT);
        return MAX(width, outlen);
    }
}

static size_t fmt_fp(FILE *f, long double x, int width, int prec, int flags, int c, int isld)
{
    const char *prefix = "-0X+0X 0X-0x+0x 0x";
    int preflen = 1;
    if (signbit(x))
        x = -x;
    else if (flags & FLG_SIGN)
        prefix += 3;
    else if (flags & FLG_MARK)
        prefix += 6;
    else
        prefix++, preflen = 0;

    if (!isfinite(x)) {
        const char *s = "INFinfNANnan";
        if (isnan(x)) s += 6;
        if (c & 32) s += 3;
        pad(f, ' ', width, 3+preflen, flags & ~FLG_ZERO);
        out(f, prefix, preflen);
        out(f, s, 3);
        pad(f, ' ', width, 3+preflen, flags ^ FLG_LEFT);
        return MAX(3+preflen, width);
    }

    if ((c|32) == 'a') return fmt_hexfloat(f, x, width, prec, flags, c, prefix, preflen + 2);
    return fmt_decfloat(f, x, width, prec, flags, c, prefix, preflen, isld);
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
                if (addo(&rv, fmt_fp(f, arg.f, width, prec, flags, c, st == LDBL)))
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
    ret = printf_core(0, fmt, &ap2, nl_type, nl_arg);
    if (ret != -1) {
        unsigned char tmpbuf[80];
        unsigned char *oldbuf;
        if (!f->buf_size) {
            oldbuf = f->buf;
            f->buf = tmpbuf;
            f->buf_size = sizeof tmpbuf;
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
