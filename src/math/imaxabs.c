#include <inttypes.h>

intmax_t imaxabs(intmax_t x) { return x < 0? -x : x; }
