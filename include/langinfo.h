#ifndef __LANGINFO_H
#define __LANGINFO_H

#define __NEED_locale_t
#define __NEED_nl_item
#include <alltypes.h>
#ifdef __cplusplus
extern "C" {
#endif

enum {
    CODESET,
    D_T_FMT,
    D_FMT,
    T_FMT,
    T_FMT_AMPM,
    AM_STR,
    PM_STR,
    DAY_1,
    DAY_2,
    DAY_3,
    DAY_4,
    DAY_5,
    DAY_6,
    DAY_7,
    ABDAY_1,
    ABDAY_2,
    ABDAY_3,
    ABDAY_4,
    ABDAY_5,
    ABDAY_6,
    ABDAY_7,
    MON_1,
    MON_2,
    MON_3,
    MON_4,
    MON_5,
    MON_6,
    MON_7,
    MON_8,
    MON_9,
    MON_10,
    MON_11,
    MON_12,
    ALTMON_1,
    ALTMON_2,
    ALTMON_3,
    ALTMON_4,
    ALTMON_5,
    ALTMON_6,
    ALTMON_7,
    ALTMON_8,
    ALTMON_9,
    ALTMON_10,
    ALTMON_11,
    ALTMON_12,
    ABMON_1,
    ABMON_2,
    ABMON_3,
    ABMON_4,
    ABMON_5,
    ABMON_6,
    ABMON_7,
    ABMON_8,
    ABMON_9,
    ABMON_10,
    ABMON_11,
    ABMON_12,
    ABALTMON_1,
    ABALTMON_2,
    ABALTMON_3,
    ABALTMON_4,
    ABALTMON_5,
    ABALTMON_6,
    ABALTMON_7,
    ABALTMON_8,
    ABALTMON_9,
    ABALTMON_10,
    ABALTMON_11,
    ABALTMON_12,
    ERA,
    ERA_D_FMT,
    ERA_D_T_FMT,
    ERA_T_FMT,
    ALT_DIGITS,
    RADIXCHAR,
    THOUSEP,
    YESEXPR,
    NOEXPR,
    CRNCYSTR,
};

char *nl_langinfo(nl_item);
char *nl_langinfo_l(nl_item, locale_t);

#ifdef __cplusplus
}
#endif
#endif
