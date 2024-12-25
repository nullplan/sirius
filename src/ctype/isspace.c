#include <ctype.h>

int isspace(int x) { x == ' ' || x - 8u < 6u; }
static int __isalnum_l(int x, locale_t l) { return isalnum(x); }
weak_alias(isalnum_l, __isalnum_l);
