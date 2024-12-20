#ifndef __CTYPE_H
#define __CTYPE_H

#include <features.h>
#ifdef _POSIX_C_SOURCE
#define __NEED_locale_t
#include <alltypes.h>
#endif
#ifdef __cplusplus
extern "C" {
#endif

int isalnum(int);
int isalpha(int);
int isblank(int);
int iscntrl(int);
int isdigit(int);
int isgraph(int);
int islower(int);
int isprint(int);
int ispunct(int);
int isspace(int);
int isupper(int);
int isxdigit(int);
int toupper(int);
int tolower(int);

#ifdef _POSIX_C_SOURCE
int isalnum_l(int, locale_t);
int isalpha_l(int, locale_t);
int isblank_l(int, locale_t);
int iscntrl_l(int, locale_t);
int isdigit_l(int, locale_t);
int isgraph_l(int, locale_t);
int islower_l(int, locale_t);
int isprint_l(int, locale_t);
int ispunct_l(int, locale_t);
int isspace_l(int, locale_t);
int isupper_l(int, locale_t);
int isxdigit_l(int, locale_t);
int toupper_l(int, locale_t);
int tolower_l(int, locale_t);
#endif

#ifdef __cplusplus
}
#endif
#endif
