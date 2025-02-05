#include <inttypes.h>

imaxdiv_t imaxdiv(intmax_t a, intmax_t b) { return (imaxdiv_t){a/b, a%b}; }
