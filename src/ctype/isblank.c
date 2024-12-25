#include <ctype.h>

int isblank(int x) { return x == ' ' || x == '\t'; }
static int __isblank_l(int x, locale_t l) { return isblank(x); }
weak_alias(isblank_l, __isblank_l);
