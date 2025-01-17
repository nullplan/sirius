#include <stdio.h>
#include <ctype.h>
#include <errno.h>

/* ASCII -> value table. One additional -1 for EOF.
 * Digits are their value, letters are their index + 10.
 */
static unsigned char digval[257] = {-1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,
    -1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, -1, -1, -1, -1, -1,
    -1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};


hidden uintmax_t __intscan(FILE *f, int base, uintmax_t limit)
{
    uintmax_t rv = 0;
    int c = shgetc(f);  /* lookahead character */
    /* skip leading whitespace */
    while (isspace(c))
        c = shgetc(f);

    /* parse leading sign */
    int negate = 0;
    if (c == '+' || c == '-') {
        if (c == '-') negate = 1;
        c = shgetc(f);
    }

    /* resolve base=0 and skip leading "0x" for base = 16 */
    int seen_digits = 0;
    if (base == 0) {
        if (c == '0') {
            seen_digits = 1;
            c = shgetc(f);
            if ((c|32) == 'x') {
                seen_digits = 0;
                base = 16;
                c = shgetc(f);
            } else {
                if (c != EOF) {
                    shungetc(f);
                    c = '0';
                }
                base = 8;
            }
        } else if (isdigit(c)) {
            base = 10;
        } else goto error;
    } else if (base == 16 && c == '0') {
        c = shgetc(f);
        if ((c|32) == 'x')
            c = shgetc(f);
        else
            seen_digits = 1;
    }

    /* finally read the number. Special cases for the common cases of base being a power-of-two and base = 10. */
    int overflow = 0;
    if (base == 10) {
        for (int d = digval[c + 1]; d < 10 && rv <= UINTMAX_MAX/10 && 10 * rv <= UINTMAX_MAX - d; c = shgetc(f), d = digval[c+1])
        {
            seen_digits = 1;
            rv = 10 * rv + d;
        }
    } else if (!(base & (base - 1))) {
        /* 8-bit DeBruijn number
         * 01234567
         * 00010111
         *
         * 000  0
         * 001  1
         * 010  2
         * 011  4
         * 100  7
         * 101  3
         * 110  6
         * 111  5
         */
        static const unsigned char tab[8] = {0, 1, 2, 4, 7, 3, 6, 5};
        const int sh = tab[(base * 0x17) >> 5];
        for (int d = digval[c + 1]; d < base && rv < (UINTMAX_MAX >> sh); c = shgetc(f), d = digval[c+1])
        {
            seen_digits = 1;
            rv = (rv << sh) | d;
        }
    } else {
        for (int d = digval[c+1]; d < base && rv <= UINTMAX_MAX/base && base*rv <= UINTMAX_MAX - d; c = shgetc(f), d = digval[c+1])
        {
            seen_digits = 1;
            rv = base * rv + d;
        }
    }

    /* keep reading until we run out of digits. */
    for (int d = digval[c+1]; d < base; c = shgetc(f), d = digval[c+1])
    {
        overflow = 1;
    }

    if (c != EOF) shungetc(f); /* unget last byte (which is the first byte past the number) */
    if (!seen_digits) goto error;

    if (limit & 1) {
        /* unsigned case */
        if (overflow || rv > limit) {
            rv = limit;
            errno = ERANGE;
        } else if (negate)
            rv = -rv;
    } else {
        /* signed case */
        if (negate) {
            rv = -rv;
            if (overflow || rv > limit) {
                rv = limit;
                errno = ERANGE;
            }
        } else {
            if (overflow || rv >= limit) {
                rv = ~limit;
                errno = ERANGE;
            }
        }
    }

    return rv;

error:
    shlim(f, 0);
    errno = EINVAL;
    return 0;
}
