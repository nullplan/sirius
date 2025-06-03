#include "crypt.h"
#include <string.h>

static const struct hashfunc {
    const char prefix[8];
    char *(*func)(const char *, const char *, char *);
} functions[] = {
    {"$5$", __sha256crypt},
    {"$6$", __sha512crypt},
    #if 0
    {"$1$", __md5crypt},
    {"$y$", __yescrypt},
    {"$gy$", __gyescrypt},
    {"$7$", __scrypt},
    {"$2b", __bcrypt},
    #endif
};

extern hidden char *__crypt_r(const char *pass, const char *salt, char *buf)
{
    char *rv = 0;
    const struct hashfunc *f;
    for (f = functions; f < functions + sizeof functions / sizeof *functions; f++)
        if (!strncmp(salt, f->prefix, strlen(f->prefix))) {
            rv = f->func(pass, salt, buf);
            break;
        }
    if (!rv) return "*";
    return rv;
}
