#include <stdio.h>
#include <float.h>
#include <math.h>
#include <limits.h>
#include <stdlib.h>
#include "libc.h"

static int addo(int *x, size_t a)
{
    if (a > INT_MAX - *x) return 1;
    *x += a;
    return 0;
}

static void pad(FILE *f, int c, int width, int len, int flags)
{
    if ((flags & (FLG_LEFT | FLG_ZERO)) || len >= width) return;
    len = width - len;
    while (len--)
        putc_unlocked(c, f);
}

static char *fmt_d(char *z, uint32_t x)
{
    for (       ; x; x /= 10) *--z = '0' + (x % 10);
    return z;
}

static char *fmt_x(char *z, uint32_t x, int lcase)
{
    const char *const alpha = lcase? "0123456789abcdef" : "0123456789ABCDEF";
    for (       ; x; x >>= 4) *--z = alpha[x & 15];
    return z;
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
    if (prec < 8 * (z - a - 1)) {
        uint32_t *r = a + 1 + prec/8;
        uint32_t mask = (1ul << (4 * (8 - prec % 8) - 1) << 1) - 1;
        uint32_t away = *r & mask;
        *r -= away;
        long double large = 1/LDBL_EPSILON;
        if ((*r & (mask << 1)) || (mask == UINT32_MAX && (r[-1] & 1)))
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
    __fwritex(prefix, preflen, f);
    pad(f, '0', width, olen, flags ^ FLG_ZERO);
    char *dz = digbuf + sizeof digbuf;
    char *da = fmt_x(dz, *a, c & 32);
    if (da == dz) *--da = '0';
    __fwritex(da, dz - da, f);
    if (prec > 0 || (flags & FLG_ALT))
        __fwritex(".", 1, f);
    for (const uint32_t *i = a + 1; i < z && prec > 0; i++, prec -= 8)
    {
        da = fmt_x(dz, *i, c & 32);
        while (da > digbuf) *--da = '0';
        __fwritex(digbuf, MIN(prec, 8), f);
    }
    pad(f, '0', prec, 0, 0);
    __fwritex(ea, ez - ea, f);
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
        int c, const char *prefix, int preflen, enum format fmt)
{
    static const int mant_dig_per_type[3] = {FLT_MANT_DIG, DBL_MANT_DIG, LDBL_MANT_DIG};
    /* the largest decimal expansion occurs at the type's true minimum.
     * The true minimum is 2^(MIN_EXP - MANT_DIG), and it has -(MIN_EXP - MANT_DIG)
     * decimal places. That resolves to MANT_DIG - MIN_EXP.
     * I save the digits in 32-bit units, where each contains 9 decimal digits,
     * so the array size is the above, divided by 9, rounding up.
     * For simplicity, I also always reserve 1 slot for pre-radix digits,
     * so 1 more must be added.
     */
    static const int arrsz_per_type[3] = {
        (FLT_MANT_DIG - FLT_MIN_EXP + 8)/9 + 1,
        (DBL_MANT_DIG - DBL_MIN_EXP + 8)/9 + 1,
        (LDBL_MANT_DIG - LDBL_MIN_EXP + 8)/9 + 1,
    };
    const int mant_dig = mant_dig_per_type[fmt];
    const int arrsz = arrsz_per_type[fmt];
    uint32_t big[arrsz];
    int e2;
    x = frexpl(x, &e2);
    if (x) {
        x *= 0x1p29;
        e2 -= 29;
    }
    /* if exponent is negative, we will shift right and need all the space right of the number we can get.
     * If exponent is non-negative, we will shift left after mantissa expansion.
     * So initial setting of a needs to be far enough into the array to allow for even the worst-case left shift
     * of e2 = MAX_EXP - 29, but not so far that the mantissa expansion leaves the array.
     * Since the number of digits in the left shift is so far smaller than for the right shift (because
     * log10(2) < 1/3), just using the number of mantissa digits is good enough for all
     * current types.
     */
    uint32_t *a = e2 < 0? big : big + arrsz - mant_dig; /* start of non-zero digits */
    uint32_t *z = a; /* end of non-zero digits */
    uint32_t *rp = a; /* last block in front of radix point. */
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

    int do_round = 0; /* flag for rounding */
    uint32_t *r; /* pointer to first insignificant digit */
    uint32_t mod; /* 10^(number of insignificant digits in *r) */
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
        __fwritex(prefix, preflen, f);
        pad(f, '0', width, outlen, flags ^ FLG_ZERO);
        da = fmt_d(dz, *a);
        if (da == dz) *--da = '0';
        __fwritex(da, dz - da, f);
        for (const uint32_t *i = a + 1; i <= rp; i++) {
            da = fmt_d(dz, *a);
            while (da > digbuf) *--da = '0';
            __fwritex(da, 9, f);
        }
        if (prec > 0 || (flags & FLG_ALT))
            __fwritex(".", 1, f);
        for (const uint32_t *i = rp + 1; i < z && prec > 0; i++, prec -= 9) {
            da = fmt_d(dz, *i);
            while (da > digbuf) *--da = '0';
            __fwritex(da, MIN(prec, 9), f);
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
        __fwritex(prefix, preflen, f);
        pad(f, '0', width, outlen, flags ^ FLG_ZERO);
        __fwritex(da, 1, f);
        if (prec > 0 || (flags & FLG_ALT))
            __fwritex(".", 1, f);
        __fwritex(da + 1, MIN(dz - da - 1, prec), f);
        prec -= num_dig_in_lead - 1;
        for (const uint32_t *i = a + 1; i < z && prec > 0; i++, prec -= 9)
        {
            da = fmt_d(dz, *i);
            while (da > digbuf) *--da = '0';
            __fwritex(da, MIN(9, prec), f);
        }
        pad(f, '0', prec, 0, 0);
        __fwritex(ea, ez - ea, f);
        pad(f, ' ', width, outlen, flags ^ FLG_LEFT);
        return MAX(width, outlen);
    }
}

hidden size_t __fmt_fp(FILE *f, long double x, int width, int prec, int flags, int c, enum format fmt)
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
        __fwritex(prefix, preflen, f);
        __fwritex(s, 3, f);
        pad(f, ' ', width, 3+preflen, flags ^ FLG_LEFT);
        return MAX(3+preflen, width);
    }

    if ((c|32) == 'a') return fmt_hexfloat(f, x, width, prec, flags, c, prefix, preflen + 2);
    return fmt_decfloat(f, x, width, prec, flags, c, prefix, preflen, fmt);
}
