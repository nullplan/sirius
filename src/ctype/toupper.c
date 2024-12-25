#include <ctype.h>

int toupper(int x) { return x - 'a' < 26u? x - 32 : x; }
static int __toupper_l(int x, locale_t l) { return toupper(x); }
weak_alias(toupper_l, __toupper_l);
