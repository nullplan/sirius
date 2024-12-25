#include <ctype.h>

int isprint(int x) { return x - 0x20u < 0x7f - 0x20u; }
static int __isprint_l(int x, locale_t l) { return isprint(x); }
weak_alias(isprint_l, __isprint_l);
