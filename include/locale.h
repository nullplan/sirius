#ifndef __LOCALE_H
#define __LOCALE_H

#include <features.h>
#define __NEED_NULL
#ifdef _POSIX_C_SOURCE
#define __NEED_locale_t
#endif
#include <alltypes.h>
#ifdef __cplusplus
extern "C" {
#endif

struct lconv {
    char *decimal_point;
    char *thousands_sep;
    char *grouping;
    char *mon_decimal_point;
    char *mon_thousands_sep;
    char *mon_grouping;
    char *positive_sign;
    char *negative_sign;
    char *currency_symbol;
    char frac_digits;
    char p_cs_precedes;
    char n_cs_precedes;
    char p_sep_by_space;
    char n_sep_by_space;
    char p_sign_posn;
    char n_sign_posn;
    char *int_curr_symbol;
    char int_frac_digits;
    char int_p_cs_precedes;
    char int_n_cs_precedes;
    char int_p_sep_by_space;
    char int_n_sep_by_space;
    char int_p_sign_posn;
    char int_n_sign_posn;
};

#define LC_COLLATE  0
#define LC_CTYPE    1
#define LC_MONETARY 2
#define LC_NUMERIC  3
#define LC_TIME     4
#define LC_MESSAGES 5
#define LC_ALL      6

char *setlocale(int, const char *);
struct lconv *localeconv(void);

#ifdef _POSIX_C_SOURCE
#define LC_COLLATE_MASK     1
#define LC_CTYPE_MASK       2
#define LC_MONETARY_MASK    4
#define LC_NUMERIC_MASK     8
#define LC_TIME_MASK        16
#define LC_MESSAGES_MASK    32
#define LC_ALL_MASK         63
#define LC_GLOBAL_LOCALE    ((locale_t)-1)
locale_t duplocale(locale_t);
void freelocale(locale_t);
const char *getlocalename_l(int, locale_t);
locale_t newlocale(int, const char *, locale_t);
locale_t uselocale(locale_t);
#endif
#ifdef __cplusplus
}
#endif
#endif
