#include <stdio.h>
#include <float.h>
#include <math.h>
#include <stdint.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>

struct getint_data {
    int err;
    int val;
};

static struct getint_data getint(FILE *f, int pseudo)
{
    int esign = -1;
    int gotsign = 0;
    int s = shgetc(f);
    if (s == '+' || s == '-') {
        gotsign = 1;
        if (s == '-') esign = 1;
        s = shgetc(f);
    }
    if (!isdigit(s)) {
        if (pseudo) {
            if (s != EOF) shungetc(f);
            if (gotsign) shungetc(f);
            shungetc(f);
            return (struct getint_data){0, 0};
        } else {
            shlim(f, 0);
            return (struct getint_data){1, 0};
        }
    } else {
        int e2 = 0;
        while (isdigit(s)) {
            if (e2 >= INT_MIN/10)
                e2 = 10 * e2 - (s - '0');
            else
                e2 = INT_MIN;
        }
        if (s != EOF)
            shungetc(f);
        if (e2 == INT_MIN && esign < 0)
            e2 = INT_MAX;
        else
            e2 *= esign;
        return (struct getint_data){0, e2};
    }
}

/* here we only need enough space for the mantissa plus two bits for the
 * rounding decision.
 * We have already skipped all leading whitespace, the sign, and the initial
 * "0x", so we still need to fail to match if the very next character is not a
 * hex digit.
 */
static long double hexfloat(FILE *f, int pseudo, int bits, int emin, int sign)
{
    int c = shgetc(f);
    long double y;
    uint32_t big[(LDBL_MANT_DIG + 33) / 32];
    int k = 0;      /* number of digits in the current number */
    size_t idx = 0; /* index of current number */
    int radix;      /* position of the radix relative to the start of the number */
    int digitcount = 0;
    int gotdig = 0;
    int gotrad = 0;

    while (c == '0') {
        gotdig = 1;
        c = shgetc(f);
    }

    if (c == '.') {
        gotrad = 1;
        radix = 0;
        c = shgetc(f);
        while (c == '0') {
            gotdig = 1;
            radix -= 4;
            c = shgetc(f);
        }
    }

    while (c == '.' || isxdigit(c)) {
        if (c == '.') {
            if (gotrad) break;
            gotrad = 1;
            radix = 4 * digitcount;
        } else if (digitcount < sizeof big * 2) {
            int d = c - '0' < 10u? c - '0' : (c|32) - 'a' + 10;
            gotdig = 1;
            if (!k) big[idx] = d;
            else big[idx] = (big[idx] << 4) | d;
            if (++k == 8) {
                idx++;
                k = 0;
            }
            digitcount++;
        } else {
            if (c != '0') big[idx-1] |= 1;
            if (digitcount < INT_MAX/4)
                digitcount++;
        }
        c = shgetc(f);
    }

    if (!gotrad) radix = 4 * digitcount;
    if (!gotdig) {
        shlim(f, 0);
        return 0;
    }

    if (k) {
        big[idx++] <<= 4 * (8 - k);
    }
    
    radix -= 32 * idx;

    if ((c|32) == 'p') {
        struct getint_data e2 = getint(f, pseudo);
        if (e2.err) return 0;
        if (radix > 0 && e2.val > INT_MAX - radix)
            radix = INT_MAX;
        else if (radix < 0 && e2.val < INT_MIN - radix)
            radix = INT_MIN;
        else
            radix += e2.val;
    } else if (c != EOF)
        shungetc(f);
    if (radix < emin - bits || radix > -emin)
    {
        if (pseudo) errno = ERANGE;
        return scalblnl(sign, radix);
    }
    y = 0;
    size_t i;
    for (i = 0; i < idx && bits >= 32; i++, bits -= 32)
        y = 0x1p32 * y + big[i];
    if (i < idx && bits) {
        y = 0x1p32 * y + (big[i] & (1ul << (32 - bits)));
        big[i] &= (1ul << (32 - bits)) - 1;
        if (sign < 0) y = -y - big[i];
        else y += big[i];
    } else y *= sign;
    return scalbln(y, radix);
}

/* In the worst case, the number given is exactly halfway between two
 * representable numbers. In that case we need to keep reading the number to
 * know which way to round. The worst case where this can happen is for
 * LDBL_TRUE_MIN/2, which is 2^(LDBL_MIN_EXP - LDBL_MANT_DIG - 1), which has
 * LDBL_MANT_DIG - LDBL_MIN_EXP + 1 decimal digits.
 *
 * The size of the big array is that number divided by 9, rounded up to the next power of two,
 * to allow for wraparound semantics with a bitmask.
 */
#if LDBL_MANT_DIG == 53
#define THRESH 9007199, 254740992
#define MASK 127
#elif LDBL_MANT_DIG == 64
#define THRESH 18, 446744073, 709551616
#define MASK 2047
#else
#define THRESH 10384593, 717069655, 257060992, 658440192
#define MASK 2047
#endif

static int b1b_cmp(const uint32_t *thresh, const uint32_t *big, size_t a, size_t n_thresh)
{
    for (size_t i = 0; i < n_thresh; i++) {
        if (big[(a+i)&MASK] != thresh[i])
            return thresh[i] < big[(a+i)&MASK]? -1 : 1;
    }
    return 0;
}

#define BILLION 1000000000
static long double decfloat(FILE *f, int pseudo, int bits, int emin, int sign, int c)
{
    uint32_t big[MASK+1];
    static const uint32_t thresh[] = { THRESH };
    const int n_thresh = sizeof thresh / sizeof *thresh;
    size_t a, z;
    int radix;
    int digitcount = 0;
    int lastnonzero = 0;
    int gotdig = 0;
    int gotrad = 0;
    int k = 0;

    while (c == '0') {
        gotdig = 1;
        c = shgetc(f);
    }

    if (c == '.') {
        gotrad = 1;
        c = shgetc(f);
        radix = 0;
        while (c == '0') {
            gotdig = 1;
            radix--;
            c = shgetc(f);
        }
    }

    a = z = 0;
    big[a] = 0;

    while (isdigit(c) || c == '.') {
        if (c == '.') {
            if (gotrad) break;
            gotrad = 1;
            radix = digitcount;
        } else if (digitcount < 9 * MASK) {
            int d = c - '0';
            if (!k) big[z] = d;
            else big[z] = 10 * big[z] + d;
            if (++k == 9) {
                k = 0;
                z++;
            }
            digitcount++;
            if (d) lastnonzero = digitcount;
        } else {
            if (c != '0') {
                big[z-1] |= 1;
                lastnonzero = digitcount;
            }
            digitcount++;
        }
        c = shgetc(f);
    }

    if (!gotrad) radix = digitcount;
    if (!gotdig) {
        shlim(f, 0);
        return 0;
    }

    if ((c|32) == 'e') {
        struct getint_data e10 = getint(f, pseudo);
        if (e10.err) return 0;
        if (radix > 0 && e10.val > INT_MAX - radix)
            radix = INT_MAX;
        else if (radix < 0 && e10.val < INT_MIN - radix)
            radix = INT_MIN;
        else
            radix += e10.val;
    } else if (c != EOF)
        shungetc(f);

    /* optimize small integers and take care of 0*/
    if (lastnonzero <= 9 && radix == lastnonzero)
        return sign * 1.0L * big[0];

    /* take care of hard overflow */
    if (radix < LDBL_MIN_10_EXP - LDBL_MANT_DIG || radix > LDBL_MAX_10_EXP) {
        if (pseudo) errno = ERANGE;
        return scalblnl(sign, radix < 0? LDBL_MIN_EXP - LDBL_MANT_DIG - 1 : LDBL_MAX_EXP);
    }

    /* now we know that radix is inside of some sensible limit. */
    if (k) {
        for (; k < 9; k++, big[z] *= 10);
        z++;
    }
    if (lastnonzero < 9 * z)
        z = (lastnonzero + 8)/9;

    int adj = radix % 9;
    if (adj) {
        if (adj < 0) adj += 9;
        uint64_t carry = 0;
        uint32_t mul = 1;
        for (int i = 0; i < adj; i++, mul *= 10);
        for (size_t i = z; i != a;)
        {
            i = (i - 1) & MASK;
            carry += (uint64_t)big[i] * mul;
            big[i] = carry % BILLION;
            carry /= BILLION;
        }
        if (carry) {
            a = (a - 1) & MASK;
            if (a == z) {
                z = (z - 1) & MASK;
                if (big[z])
                    big[(z-1)&MASK] |= 1;
            }
            big[a] = carry;
            radix += 9;
        }
        radix -= adj;
    }

    int e2 = 0;
    while (radix <= 9 * n_thresh) {
        int shiftcount = 29; // XXX: Find better way to do this
        uint64_t carry = 0;
        for (size_t i = z; i != a;) {
            i = (i - 1) & MASK;
            carry += (uint64_t)big[i] << shiftcount;
            big[i] = carry % BILLION;
            carry /= BILLION;
        }
        while (((z-1)&MASK) != a && !big[(z-1)&MASK])
            z = (z-1)&MASK;

        if (carry) {
            a = (a - 1) & MASK;
            if (a == z) {
                z = (z - 1) & MASK;
                if (big[z])
                    big[(z-1)&MASK] |= 1;
            }
            big[a] = carry;
            radix += 9;
        }
        e2 -= shiftcount;
    }
    
    while (radix > 9 * n_thresh || (radix == 9 * n_thresh && b1b_cmp(thresh, big, a, n_thresh) <= 0)) {
        int shiftcount = 1; // XXX: Find a better way to do this
        uint32_t carry_out, carry_in = 0;
        uint32_t mask = (1ul << shiftcount) - 1;
        for (size_t i = a; i != z; i = (i + 1) & MASK) {
            carry_out = big[i] & mask;
            big[i] = (big[i] >> shiftcount) + (BILLION >> shiftcount) * carry_in;
            carry_in = carry_out;
        }
        while (!big[a] && ((a+1)&MASK) != z)
        {
            a = (a+1)&MASK;
            radix -= 9;
        }
        if (carry_out)
        {
            if (((z+1)&MASK) == a) big[(z-1)&MASK] |= 1;
            else {
                big[z] = (BILLION>>shiftcount) * carry_out;
                z = (z + 1) & MASK;
            }
        }
        e2 += shiftcount;
    }
    /* there are exactly "bits" many bits worth of data in 
     * in the mantissa. The exponent on the lead digit will
     * be bits+e2. If that's larger than -emin, that's an overflow.
     */
    if (e2 > bits-emin || e2 < emin) {
        if (pseudo) errno = ERANGE;
        return scalbln(sign, bits+e2);
    }

    long double y = 0;
    for (size_t i = 0; i < n_thresh; i++)
        y = BILLION * y + big[(a+i)&MASK];
    long double small;
    if (z == ((a + n_thresh) & MASK)) small = 0;
    else if (big[(a+n_thresh)&MASK] < (1ul << 31)) small = 0.25;
    else if (big[(a+n_thresh)&MASK] == (1ul << 31) && z == ((a + n_thresh + 1) & MASK)) small = 0.5;
    else small = 0.75;
    if (sign < 0) {
        y = -y;
        small = -small;
    }
    return scalblnl(y+small, e2);
}

hidden long double __floatscan(FILE *f, int pseudo, int bits, int emin)
{
    int c = shgetc(f);
    while (isspace(c)) c = shgetc(f);
    int sign = 1;
    if (c == '+' || c == '-') {
        if (c == '-') sign = -1;
        c = shgetc(f);
    }
    size_t i;
    for (i = 0; i < 8 && (c|32) == "infinity"[i]; i++)
        if (i < 7) c = shgetc(f);
    if (i == 3 || i == 8 || (pseudo && i > 3)) {
        if (i - 4u < 4u) {
            for (; i > 3; i--, shungetc(f));
        }
        return sign * INFINITY;
    } else if (i) {
        shlim(f, 0);
        return 0;
    }

    for (i = 0; i < 3 && (c|32) == "nan"[i]; i++)
        c = shgetc(f);

    if (i == 3) {
        if (c == '(') {
            for (i = 0; c != ')' && c != EOF; i++, c = shgetc(f));
            if (c == ')') return sign * NAN;
            if (!pseudo) {
                shlim(f, 0);
                return 0;
            }
            for (; i > 0; i--, shungetc(f));
        }
        return sign * NAN;
    } else if (i) {
        shlim(f, 0);
        return 0;
    }

    if (c == '0') {
        c = shgetc(f);
        if ((c|32) == 'x')
            return hexfloat(f, pseudo, bits, emin, sign);
        if (c != EOF)
            shungetc(f);
        c = '0';
    }
    return decfloat(f, pseudo, bits, emin, sign, c);
}
