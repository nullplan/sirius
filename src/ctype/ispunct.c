#include <ctype.h>

int ispunct(int x) { return x - 0x21u < 15u || x - 0x3au < 7u || x - 0x5bu < 6u || x - 0x7bu < 4u; }
static int __ispunct_l(int x, locale_t l) { return ispunct(x); }
weak_alias(ispunct_l, __ispunct_l);
