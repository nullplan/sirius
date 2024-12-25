#include <ctype.h>

int isupper(int x) { return x - 'A' < 26u; }
static int __isupper_l(int x, locale_t l) { return isupper(x); }
weak_alias(isupper_l, __isupper_l);
