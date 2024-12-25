#include <ctype.h>

int isgraph(int x) { return x - 0x21u < 0x7f - 0x21u; }
static int __isgraph_l(int x, locale_t l) { return isgraph(x); }
weak_alias(isgraph_l, __isgraph_l);
