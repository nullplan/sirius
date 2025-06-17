#include "libm.h"
#include "cpu.h"

/* Using fixed-point Goldschmidt's algorithm.
 * On a high level: 1st weed out special arguments.
 * 2nd normalize number with frexp()
 * 3rd if exponent is odd, halve mantissa and increase exponent.
 *
 * Mantissa is now between 1/4 and 1. Use Goldschmidt's algorithm to
 * calculate square root of mantissa, then scale result
 * with half the exponent.
 *
 * Using 64-bit integers for the calculation to converge further
 * than the 53 bit mantissa we need, so we can round correctly.
 */

/* multiply two 2.62 fixed point numbers:
 * Essentially, calculate the 128 bit result, and return
 * that, right shifted by 62.
 * Some CPUs might have a fast 64x64->128 multiplication instruction,
 * so I'm moving the nitty-gritty of that into cpu.h.
 */
static uint64_t mul64(uint64_t a, uint64_t b)
{
    struct uint128 r = a_mul128(a, b);
    return r.hi << 2 | r.lo >> 62;
}

static uint64_t inner_sqrt(uint64_t s)
{
    uint64_t b = s;
    uint64_t t = (__sqrt_data[(s >> 55) - 0x20] + 0ull) << 56;
    uint64_t x = mul64(s, t);

    for (int i = 0; i < 4; i++) {
        b = mul64(t, mul64(t, b));
        t = ((3ull << 62) - b) >> 1;
        x = mul64(x, t);
    }
    return x;
}

double sqrt(double x)
{
    /* special arguments:
     * NaN, -0, +0, ∞ -> return x
     * -∞, -x -> return NaN, raise invalid
     */
    if (x < 0.0) return (x-x)/(x-x);
    if (!isfinite(x) || x == 0.0) return x;

    int e2;
    x = frexp(x, &e2);
    uint64_t ix = __double_bits(x) & ((1ull << 52) - 1);
    ix |= 1ull << 52;
    ix <<= 8;
    if (e2 & 1) {
        e2++;
        ix >>= 1;
    }
    return scalbln(inner_sqrt(ix), e2 / 2 - 62);
}
