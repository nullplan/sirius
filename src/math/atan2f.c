#include "libm.h"

#ifdef __GNUC__
#define fabsf(x) __builtin_fabsf(x)
#endif

static const float pi    = 0x1.921fb4P1f;
static const float pi_lo = 0x1.4442d1P-23;
static const float tiny  = 0x1.0P-116f;

float atan2f(float y, float x)
{
    if (isnan(y) || isnan(x)) return x+y;
    if (y == 0) {
        if (signbit(x)) return y;
        return copysignf(pi+tiny, y);
    }
    if (x == 0) return copysignf(0.5f * pi + tiny, y);
    if (isinf(x)) {
        if (isinf(y)) return copysignf(signbit(x)? 0.75f * pi + tiny : 0.25f * pi + tiny, y);
        return copysignf(signbit(x)? pi + tiny: 0, y);
    }
    if (isinf(y)) return copysignf(0.5f * pi + tiny, y);

    float z = atanf(fabs(y/x));
    if (signbit(x)) z = pi - (z - pi_lo);
    if (signbit(y)) z = -z;
    return z;
}
