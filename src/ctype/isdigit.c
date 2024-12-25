#include <ctype.h>

int isdigit(int x) { return x - '0' < 10u; }
static int __isdigit_l(int x, locale_t l) { return isdigit(x); }
weak_alias(isdigit_l, __isdigit_l);
