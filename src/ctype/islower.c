#include <ctype.h>

int islower(int x) { return x - 'a' < 26u; }
static int __islower_l(int x, locale_t l) { return islower(x); }
weak_alias(islower_l, __islower_l);
