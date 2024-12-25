#include <ctype.h>

int iscntrl(int x) { return x < 0x20u || x == 0x7f; }
static int __iscntrl_l(int x, locale_t l) { return iscntrl(x); }
weak_alias(iscntrl_l, __iscntrl_l);
