#include <ctype.h>

int (isxdigit)(int x) { return x - '0' < 10u || (x|32) - 'a' < 6u; }
static int __isxdigit_l(int x, locale_t l) { return isxdigit(x); }
weak_alias(isxdigit_l, __isxdigit_l);
