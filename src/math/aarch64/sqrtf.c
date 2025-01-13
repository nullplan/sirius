#include <math.h>

float sqrtf(float x) {
    __asm__("fsqrt %0, %1" : "=w"(x) : "w"(x));
    return x;
}
