#include "libm.h"
#pragma STDC FENV_ACCESS ON

double fromfp(double x, int rnd, unsigned width)
{
    uint64_t ix = __double_bits(x);
    int exp = ((ix >> 52) & 0x7ff) - 0x3ff;
    double y = x;
    if (exp < 0) {
        /* trivial case: All bits fractional. */
        switch (rnd) {
            case FP_INT_TONEAREST:
                if (exp == -1 && (ix & (1ull << 52 - 1))) y = ix >> 63? -1 : 1;
                else y = ix >> 63? -0.0 : 0;
                break;

            case FP_INT_TONEARESTFROMZERO:
                if (exp == -1) y = ix >> 63? -1 : 1;
                else y = ix >> 63? -0.0 : 0;
                break;

            case FP_INT_TOWARDZERO:
                y = ix >> 63? -0.0 : 0;
                break;

            case FP_INT_UPWARD:
                y = ix >> 63? -0.0 : 1;
                break;

            case FP_INT_DOWNWARD:
                y = ix >> 63? -1 : 0;
                break;
        }
    } else if (exp < 52) {
        uint64_t fractmask = (1ull << (52 - exp)) - 1;
        switch (rnd) {
            case FP_INT_TONEAREST:
                ix += fractmask >> 1;
                ix += (ix >> (53 - exp)) & 1;
                break;

            case FP_INT_TONEARESTFROMZERO:
                ix += (fractmask >> 1) + 1;
                break;

            case FP_INT_TOWARDZERO:
                break;

            case FP_INT_UPWARD:
                if (!(ix >> 63)) ix += fractmask;
                break;

            case FP_INT_DOWNWARD:
                if (ix >> 63) ix += fractmask;
                break;
        }
        y = __double_from_bits(ix & ~fractmask);
    }

    /* now detect overflow */
    uint64_t iy = __double_bits(y);
    exp = ((iy >> 52) & 0x7ff) - 0x3ff;
    if (exp >= 0 && (exp > (width - 1) || (exp == (width - 1) && iy != (0xbfeULL + width) << 52)))
        y = (y - y)/(y - y);
    return y;
}
