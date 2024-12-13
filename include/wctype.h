#ifndef __WCTYPE_H
#define __WCTYPE_H

#define __NEED_wint_t
#define __NEED_WEOF
#include <alltypes.h>
#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef int wctrans_t;
typedef int wctype_t;

int iswalnum(wint_t);
int iswalpha(wint_t);
int iswblank(wint_t);
int iswcntrl(wint_t);
int iswgraph(wint_t);
int iswlower(wint_t);
int iswprint(wint_t);
int iswpunct(wint_t);
int iswspace(wint_t);
int iswupper(wint_t);
int iswxdigit(wint_t);
int iswctype(wint_t, wctype_t);
wctype_t wctype(const char *);
wint_t towlower(wint_t);
wint_t towupper(wint_t);
wint_t towctrans(wint_t, wctrans_t);
wctrans_t wctrans(const char *);

#ifdef __cplusplus
}
#endif
#endif
