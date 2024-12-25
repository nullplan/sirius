#include <ctype.h>

int isalpha(int x) { return islower(x|32); }
static int __isalpha_l(int x, locale_t l) { return isalpha(x); }
weak_alias(isalpha_l, __isalpha_l);
