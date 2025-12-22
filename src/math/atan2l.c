#include "libm.h"
#pragma STDC FENV_ACCESS ON

#if LDBL_MANT_DIG==53
long double atan2l(long double y, long double x)
{
    return atan2(y, x);
}
#else
#ifdef __GNUC__
#define fabsl(x) __builtin_fabsl(x)
#endif
/* π =
 * 11.0010010000111111011010101000100010000101101000110000100011010011000100110001100110001010001011100000001101110000011100110100010010100100000010010011100000100010001010011001111100110001110100000000100000101110111110101001100011101100010011100110110010001001010001010010100000100001111001100011100011010000000100110111011110111110010101000110011011001111001101001110100100001100011011001100000010101100001010011011011111001001011111000101000011011101001111111000010011010101101101011011010101000111000010010001..._2
 */
#if LDBL_MANT_DIG==64
static const long double pi    = 0x1.921fb54442d18468P1L;
static const long double pi_lo = 0x1.13198a2e03707344P-63L;
static const long double tiny  = 0x1.0P-16352L;
#elif LDBL_MANT_DIG==113
/* π broken into two words:
 * 1.1001 0010 0001 1111 1011 0101 0100 0100 0100 0010 1101 0001 1000 0100 0110 1001 1000 1001 1000 1100 1100 0101 0001 0111 0000 0001 1011 1000P1
 * 1.1100 1101 0001 0010 1001 0000 0010 0100 1110 0000 1000 1000 1010 0110 0111 1100 1100 0111 0100 0000 0010 0000 1011 1011 1110 1010 0110 0011P-114
 */
static const long double pi    = 0x1.921fb54442d18469898cc51701b8P1L;
static const long double pi_lo = 0x1.cd129024e088a67cc74020bbea63P-114L;
static const long double tiny  = 0x1.0P-16320L;
#endif

long double atan2l(long double y, long double x)
{
    if (isnan(y) || isnan(x)) return x+y;
    if (y == 0) {
        if (signbit(x)) return y;
        return copysignl(pi+tiny, y);
    }
    if (x == 0) return copysignl(0.5 * pi + tiny, y);
    if (isinf(x)) {
        if (isinf(y)) return copysignl(signbit(x)? 0.75 * pi + tiny : 0.25 * pi + tiny, y);
        return copysignl(signbit(x)? pi + tiny : 0, y);
    }
    if (isinf(y)) return copysign(0.5 * pi + tiny, y);
    double z = atanl(fabsl(y/x));
    if (signbit(x)) z = pi - (z - pi_lo);
    if (signbit(y)) z = -z;
    return z;
}
#endif
