#include <netdb.h>
#include <stdint.h>
#include <stddef.h>

struct errtab {
    #define E(n, m) char msg##n[sizeof m];
    #include "gai_strerror.h"
    #undef E
};

static const struct errtab errtab = {
    #define E(n, m) .msg##n = m,
    #include "gai_strerror.h"
    #undef E
};

static const uint16_t offs[] = {
    #define E(n, m) [-n] = offsetof(struct errtab, msg##n),
    #include "gai_strerror.h"
    #undef E
};

const char *gai_strerror(int e)
{
    e = -e;
    if (e + 0ul >= sizeof offs / sizeof *offs)
        e = 0;
    return (const char *)&errtab + offs[e];
}
