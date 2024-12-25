#include <ctype.h>

int isalnum(int x) { return islower(x|32) || isdigit(x); }
static int __isalnum_l(int x, locale_t l) { return isalnum(x); }
weak_alias(isalnum_l, __isalnum_l);
