#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <errno.h>

#ifdef __GNUC__
#define unlikely(x) __builtin_expect((x), 0)
#else
#define unlikely(x) (x)
#endif

struct errtab {
    #define E(code, str) char str_##code [sizeof str];
    #include "strerror.h"
    #undef E
};

static const struct errtab errtab = {
    #define E(code, str) .str_##code = str,
    #include "strerror.h"
    #undef E
};

static const uint16_t erroff[] = {
    #define E(code, str) [code] = offsetof(struct errtab, str_##code) + 1,
    #include "strerror.h"
};

char *strerror(int e)
{
    if (unlikely(e >= sizeof erroff/sizeof *erroff || !erroff[e])) return "Unknown error";
    return ((char *)&errtab) + erroff[e] - 1;
}

static char *__strerror_l(int e, locale_t l)
{
    return strerror(e);
}
weak_alias(strerror_l, __strerror_l);
