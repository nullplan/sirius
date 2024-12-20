#ifndef __MONETARY_H
#define __MONETARY_H

#define __NEED_locale_t
#define __NEED_size_t
#define __NEED_ssize_t
#include <alltypes.h>
#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

ssize_t strfmon(char *__restrict, size_t, const char *__restrict, ...);
ssize_t strfmon_l(char *__restrict, size_t, locale_t, const char *__restrict, ...);

#ifdef __cplusplus
}
#endif
#endif
