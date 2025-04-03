#include "libm.h"

float atanf(float x)
{
    static const float c[] = {
        +1.0f/3.0f,
        -1.0f/5.0f,
        +1.0f/7.0f,
        -1.0f/9.0f,
        +1.0f/11.0f,
        -1.0f/13.0f,
    };
    int ex = ((__float_bits(x) >> 23) & 0xff) - 0x7f;
    if (ex >= 66) {
        if (isnan(x)) return x;
        FORCE_EVAL(0x1p54 + 0x1p-3);
        return copysign(M_PI_2, x);
    }
    if (ex < -29) {
        FORCE_EVAL(0x1p54 + x);
        return x;
    }
    int flip = signbit(x);
    if (flip) x = -x;
    int addconst = 1;
    float hiconst;
    float loconst;
    if (x < 0.4375f)
        addconst = 0;
    else if (x < 0.6875f) {
        hiconst = 0x1.dac670P-2f;           /* atan(1/2) */
        loconst = 0x1.586ed2P-28f;          /* atan(1/2) */
        x = (x - 0.5f)/(1.0f + 0.5f * x);
    } else if (x < 1.1875f) {
        hiconst = 0x1.921fb4P-1f;           /* atan(1) = π/4 */
        loconst = 0x1.4442b0P-25f;          /* atan(1) = π/4 */
        x = (x - 1.0f)/(x + 1.0f);
    } else if (x < 2.4375f) {
        hiconst = 0x1.f730bcP-1f;           /* atan(1.5) */
        loconst = 0x1.281f68P-25f;          /* atan(1.5) */
        x = (x - 1.5f)/(1.0f + 1.5f * x);
    } else {
        hiconst = 0x1.921fb4P0f;            /* π/2 */
        loconst = 0x1.4442b0P-24f;          /* π/2 */
        x = -1.0f/x;
    }

    float x2 = x * x, x4 = x2 * x2;
    float s1 = x2 * (c[0] + x4 * (c[2] + x4 * (c[4] + x4 * c[6])));
    float s2 = x4 * (c[1] + x4 * (c[3] + x4 * c[5]));
    if (!addconst) x = x - x * (s1 + s2);
    else x = hiconst - (x * (s1 + s2) - loconst - x);
    if (flip) x = -x;
    return x;
}
