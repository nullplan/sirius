#ifndef LOCALE_H
#define LOCALE_H

#include "../../include/locale.h"
#include <limits.h>

struct __localedef {
    char name[LC_ALL][NL_LANGMAX+1];
    int is_utf8;
};
extern hidden const struct __localedef __c_locale;
extern hidden const struct __localedef __c_dot_utf8_locale;

extern hidden void __set_locale_category(int, const char *, struct __localedef *);
#endif
