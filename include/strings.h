#ifndef __STRINGS_H
#define __STRINGS_H

#define __NEED_size_t
#define __NEED_locale_t
#include <alltypes.h>
#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

int strcasecmp(const char *, const char *);
int strcasecmp_l(const char *, const char *, locale_t);
int strncasecmp(const char *, const char *, size_t);
int strncasecmp_l(const char *, const char *, size_t, locale_t);

#ifdef _XOPEN_SOURCE
int ffs(int);
int ffsl(long);
int ffsll(long long);
#endif
#ifdef __cplusplus
}
#endif
#endif
