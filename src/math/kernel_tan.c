#include "libm.h"

static const double T[] = {
    1.0/3.0,
    2.0/15.0,
    17.0/315.0,
    62.0/2835.0,
    1382.0/155925.0,
    21844.0/6081075.0,
    929569.0/638512875.0,
    6404582.0/10854718875.0,
    443861162.0/1856156927625.0,
    113927491862.0/2900518163668125.0,
    58870668456604.0/3698160658676859375.0,
    8374643517010684.0/1298054391195577640625.0,
};

hidden double __kernel_tan(double x, double y, int isodd)
{
    int e = (__double_bits(x) >> 52) & 0x3ff;
    if (e < 0x3ff-27) return isodd? -1.0/(x+y) : x;
    double x2 = x * x;
    double t = T[11];
    for (int i = 10; i-- > 0;)
        t = t * x2 + T[i];
    t = x * (1 + t);
    t += (1 + x2) * y;
    if (isodd) t = -1.0/t;
    return t;
}
