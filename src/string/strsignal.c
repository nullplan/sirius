#include <string.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __GNUC__
#define unlikely(x) __builtin_expect((x), 0)
#else
#define unlikely(x) (x)
#endif

struct sigtab {
    #define S(n, s, l) char str_##n[sizeof l];
    #include "strsignal.h"
};

static const struct sigtab sigtab = {
    #undef S
    #define S(n, s, l) .str_##n = l,
    #include "strsignal.h"
};

static const uint16_t offs[_NSIG] = {
    #undef S
    #define S(n, s, l) [n] = offsetof(struct sigtab, str_##n),
    #include "strsignal.h"
};

char *strsignal(int s)
{
    if (unlikely(s >= _NSIG)) return "Unknown signal";
    return ((char *)&sigtab) + offs[s]; 
}
