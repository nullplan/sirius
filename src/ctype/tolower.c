#include <ctype.h>

int tolower(int x) { return x - 'A' < 26u? x + 32 : x; }
static int __tolower_l(int x, locale_t l) { return tolower(x); }
weak_alias(tolower_l, __tolower_l);
