#include <locale.h>
#include <string.h>
#include <limits.h>
#include "libc.h"

char *setlocale(int cat, const char *name)
{
    static char namebuf[LC_ALL * (NL_LANGMAX + 1)];
    if (cat == LC_ALL) {
        if (name) {
            /* name might be an encoded string for mismatching locale */
            if (strchr(name, ':')) {
                char *catp[LC_ALL];
                char *p = namebuf;
                size_t l = strnlen(name, sizeof namebuf);
                if (l >= sizeof namebuf) return 0;
                memcpy(namebuf, name, l + 1);
                for (size_t i = 0; i < LC_ALL; i++) {
                    catp[i] = p;
                    p += __stridx(p, ':');
                    if ((i < LC_ALL - 1 && !*p)
                            || (i == LC_ALL - 1 && *p))
                        return 0; /* too many or too few colons */
                    if (catp[i] == p) return 0; /* empty category */
                    *p++ = 0;
                }
                for (int i = 0; i < LC_ALL; i++)
                    __set_locale_category(i, catp[i], &__global_locale);
            } else {
                for (int i = 0; i < LC_ALL; i++)
                    __set_locale_category(i, name, &__global_locale);
            }
        }
        /* do we need to return an encoding? */
        int i;
        for (i = 1; i < LC_ALL; i++)
            if (strcmp(__global_locale.name[0], __global_locale.name[i]))
                break;
        if (i == LC_ALL)
            return __global_locale.name[0]; /* no, we don't */
        char *p = namebuf;
        for (i = 0; i < LC_ALL; i++) {
            size_t l = strlen(__global_locale.name[i]);
            memcpy(p, __global_locale.name[i], l);
            p[l] = ':';
            p += l + 1;
        }
        p[-1] = 0;
        return namebuf;
    } else {
        if (name) __set_locale_category(cat, name, &__global_locale);
        return __global_locale.name[cat];
    }
}
