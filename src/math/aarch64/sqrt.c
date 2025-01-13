#include <math.h>

double sqrt(double x) {
    __asm__("fsqrt %0, %1" : "=w"(x) : "w"(x));
    return x;
}
