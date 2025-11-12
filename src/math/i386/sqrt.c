#include "libm.h"

double sqrt(double x)
{
    /* union ldshape is suboptimal for i386, due to its use of uint64_t */
    union {
        long double f;
        uint32_t mant;
    } lds;
    uint16_t sw;
    __asm__("fsqrt; fstsw %1" : "=t"(lds.f), "=am"(sw) : "0"(x) : "memory");
    /* fix double rounding issue.
     * fsqrt rounds once from the infinitely-precise result to
     * long double. Then this function again rounds to double.
     * If the first rounding to long double hits the exact half-way case,
     * we must change the tail of the number in the opposite
     * direction of the previous rounding to round the right way.
     * We cannot hit the half-way case with the infinitely-precise
     * result, because the square of a number with that tail cannot
     * be represented as double.
     *
     * So if C1 is set, we must perturb the value down, and if it
     * is clear, we must perturb it up. And we must never completely
     * clear the tail.
     *
     * C1 has mask 0x200. So (sw & 0x200) + 0x300 becomes 0x300 if C1 is clear and 0x500 if it is set.
     * Then we XOR with the tail, turning the tail into 0x700 if C1 is clear and 0x200 if it is set.
     */
    if ((lds.mant & 0x7ff) == 0x400) {
        lds.mant ^= (sw & 0x200) + 0x300;
    }
    return (double)lds.f;
}
