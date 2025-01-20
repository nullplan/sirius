#ifndef LOCALE_H
#define LOCALE_H

#include "../../include/locale.h"
#include <limits.h>

struct __localedef {
    char name[LC_ALL][NL_LANGMAX+1];
    int is_utf8;
};
#endif
