#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>

struct sigtab {
    #define S(n, s, l) char str_##n[sizeof s];
    #include "strsignal.h"
};

static const struct sigtab sigtab = {
    #undef S
    #define S(n, s, l) .str_##n = s,
    #include "strsignal.h"
};

static const unsigned short offs[_NSIG] = {
    #undef S
    #define S(n, s, l) [n] = offsetof(struct sigtab, str_##n),
    #include "strsignal.h"
};

int sig2str(int sig, char *str)
{
    if (sig + 0u >= _NSIG) return -1;
    strcpy(str, (char *)&sigtab + offs[sig]);
    return 0;
}

int str2sig(const char *restrict str, int *restrict sig)
{
    for (int i = 0; i < _NSIG; i++)
        if (!strcmp((char *)&sigtab + offs[i], str)) {
            *sig = i;
            return 0;
        }
    if ((!strncmp(str, "RTMIN+", 6) || !strncmp(str, "RTMAX-", 6)) && isdigit(str[6])) {
        unsigned long n = strtoul(str + 6, 0, 10);
        /* I am supposed to recognize this form if n is in [1; RTMAX - RTMIN - 1]
         * RTMIN = 35
         * RTMAX = _NSIG - 1
         * RTMAX - RTMIN - 1 = _NSIG - 1 - 35 - 1 = _NSIG - 37
         * Interval test: x ∈ [a; b] ↔ (unsigned)x - a <= b - a ↔ (unsigned) x - a < b - a + 1
         */
        if (n - 1ul < _NSIG - 37ul) {
            *sig = str[5] == '+'? 35 + n : _NSIG - 1 - n;
            return 0;
        }
    } else if (isdigit(str[0])) {
        unsigned long n = strtoul(str, 0, 10);
        if (n <= _NSIG - 1ul) {
            *sig = n;
            return 0;
        }
    }
    return -1;
}
