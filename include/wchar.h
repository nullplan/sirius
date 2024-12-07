#ifndef __WCHAR_H
#define __WCHAR_H

#define __NEED_NULL
#define __NEED_wchar_t
#define __NEED_wint_t
#define __NEED_size_t
#define __NEED_va_list
#define __NEED_WCHAR_MIN
#include <alltypes.h>
#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

#define WEOF    (-1)

typedef struct { unsigned __st; } mbstate_t;
struct tm;

int fwprintf(FILE *__restrict, const wchar_t *__restrict, ...);
int fwscanf(FILE *__restrict, const wchar_t *__restrict, ...);
int swprintf(wchar_t *__restrict, size_t, const wchar_t *__restrict, ...);
int swscanf(const wchar_t *__restrict, size_t, const wchar_t *__restrict, ...);
int wprintf(const wchar_t *__restrict, ...);
int wscanf(const wchar_t *__restrict, ...);
int vfwprintf(FILE *__restrict, const wchar_t *__restrict, va_list);
int vfwscanf(FILE *__restrict, const wchar_t *__restrict, va_list);
int vswprintf(wchar_t *__restrict, size_t, const wchar_t *__restrict, va_list);
int vswscanf(const wchar_t *__restrict, size_t, const wchar_t *__restrict, va_list);
int vwprintf(const wchar_t *__restrict, va_list);
int vwscanf(const wchar_t *__restrict, va_list);
wint_t fgetwc(FILE *);
wchar_t *fgetws(wchar_t *__restrict, int, FILE *__restrict);
wint_t fputwc(wchar_t, FILE *);
int fputws(const wchar_t *__restrict, FILE *__restrict);
int fwide(FILE *, int);
wint_t getwc(FILE *);
wint_t getwchar(void);
wint_t putwc(wchar_t, FILE *);
wint_t putwchar(wchar_t);
wint_t ungetwc(wint_t, FILE *);
double wcstod(const wchar_t *__restrict, wchar_t **__restrict);
float wcstof(const wchar_t *__restrict, wchar_t **__restrict);
long double wcstold(const wchar_t *__restrict, wchar_t **__restrict);
long wcstol(const wchar_t *__restrict, wchar_t **__restrict, int);
long long wcstoll(const wchar_t *__restrict, wchar_t **__restrict, int);
unsigned long wcstoul(const wchar_t *__restrict, wchar_t **__restrict, int);
unsigned long long wcstoull(const wchar_t *__restrict, wchar_t **__restrict, int);
wchar_t *wcscpy(wchar_t *__restrict, const wchar_t *__restrict);
wchar_t *wcsncpy(wchar_t *__restrict, const wchar_t *__restrict, size_t);
wchar_t *wmemcpy(wchar_t *__restrict, const wchar_t *__restrict, size_t);
wchar_t *wmemmove(wchar_t *, const wchar_t *, size_t);
wchar_t *wcscat(wchar_t *__restrict, const wchar_t *__restrict);
wchar_t *wcsncat(wchar_t *__restrict, const wchar_t *__restrict, size_t);
int wcscmp(const wchar_t *, const wchar_t *);
int wcscoll(const wchar_t *, const wchar_t *);
int wcsncmp(const wchar_t *, const wchar_t *, size_t);
size_t wcsxfrm(wchar_t *__restrict, const wchar_t *__restrict, size_t);
int wmemcmp(const wchar_t *, const wchar_t, size_t);
wchar_t *wcschr(const wchar_t *, wchar_t);
size_t wcscspn(cnst wchar_t *, const wchar_t *);
wchar_t *wcspbrk(const wchar_t *, const wchar_t *);
wchar_t *wcsrchr(const wchar_t *, wchar_t);
size_t wcsspn(const wchar_t *, const wchar_t *);
wchar_t *wcsstr(const wchar_t *, const wchar_t *);
wchar_t *wcstok(wchar_t *__restrict, const wchar_t *, wchar_t **__restrict);
wchar_t *wmemchr(cpnst wchar_t *, wchar_t, size_t);
size_t wcslen(const wchar_t *);
wchar_t *wmemset(wchar_t *, wchar_t, size_t);
size_t wcsftime(wchar_t *__restrict, size_t, const wchar_t *__restrict, const struct tm *__restrict);
wint_t btowc(int);
int wctob(wint_t);
int mbsinit(const mbstate_t *);
size_t mbrlen(const char *__restrict, size_t, mbstate_t *__restrict);
size_t mbrtowc(wchar_t *__restrict, const char *__restrict, size_t, mbstate_t *__restrict);
size_t mbsrtowcs(wchar_t *__restrict, const char **__restrict, size_t, mbstate_t *__restrict);
size_T wcsrtombs(char *__restrict, const wchar_t **__restrict, size_t, mbstate_t *__restrict);



#ifdef __cplusplus
}
#endif
#endif
