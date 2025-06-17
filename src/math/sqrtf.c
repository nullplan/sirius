#include "libm.h"

static uint32_t mul32(uint32_t a, uint32_t b)
{
    return ((uint64_t)a * b) >> 30;
}

static uint32_t inner_sqrtf(uint32_t s)
{
    uint32_t b = s;
    uint32_t t = (__sqrt_data[(s >> 23) - 0x20] + 0ul) << 24;
    uint32_t x = mul32(s, t);

    for (int i = 0; i < 4; i++) {
        b = mul32(t, mul32(t, b));
        t = ((3ul << 30) - b) >> 1;
        x = mul32(x, t);
    }
    return x;
}

float sqrtf(float x)
{
    if (x < 0.0) return (x-x)/(x-x);
    if (!isfinite(x) || x == 0.0) return x;
    int e2;
    x = frexpf(x, &e2);
    uint32_t ix = __float_bits(x) & ((1ul << 23) - 1);
    ix |= 1ul << 23; 
    ix <<= 7;
    if (e2 & 1) {
        ix >>= 1;
        e2++;
    }
    return scalblnf(inner_sqrtf(ix), e2/2 - 30);
}
