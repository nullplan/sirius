#ifndef __WCHAR_H
#define __WCHAR_H

#include <features.h>
#define __NEED_NULL
#define __NEED_wchar_t
#define __NEED_wint_t
#define __NEED_size_t
#define __NEED_va_list
#define __NEED_FILE
#define __NEED_WCHAR_MIN
#define __NEED_mbstate_t
#ifdef _POSIX_C_SOURCE
#define __NEED_locale_t
#endif
#include <alltypes.h>
#ifdef __cplusplus
extern "C" {
#endif

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
size_t wcscspn(const wchar_t *, const wchar_t *);
wchar_t *wcspbrk(const wchar_t *, const wchar_t *);
wchar_t *wcsrchr(const wchar_t *, wchar_t);
size_t wcsspn(const wchar_t *, const wchar_t *);
wchar_t *wcsstr(const wchar_t *, const wchar_t *);
wchar_t *wcstok(wchar_t *__restrict, const wchar_t *, wchar_t **__restrict);
wchar_t *wmemchr(const wchar_t *, wchar_t, size_t);
size_t wcslen(const wchar_t *);
wchar_t *wmemset(wchar_t *, wchar_t, size_t);
size_t wcsftime(wchar_t *__restrict, size_t, const wchar_t *__restrict, const struct tm *__restrict);
wint_t btowc(int);
int wctob(wint_t);
int mbsinit(const mbstate_t *);
size_t mbrlen(const char *__restrict, size_t, mbstate_t *__restrict);
size_t mbrtowc(wchar_t *__restrict, const char *__restrict, size_t, mbstate_t *__restrict);
size_t mbsrtowcs(wchar_t *__restrict, const char **__restrict, size_t, mbstate_t *__restrict);
size_t wcrtomb(char *__restrict, wchar_t, mbstate_t *__restrict);
size_t wcsrtombs(char *__restrict, const wchar_t **__restrict, size_t, mbstate_t *__restrict);

#if __STDC_VERSION__ >= 202311L
#define wcschr(s, c) _Generic((1? (s) : (void *)0), const void *: (const wchar_t *)wcschr((s), (c)), void *: wcschr((s), (c)))
#define wcspbrk(s, c) _Generic((1? (s) : (void *)0), const void *: (const wchar_t *)wcspbrk((s), (c)), void *: wcspbrk((s), (c)))
#define wcsrchr(s, c) _Generic((1? (s) : (void *)0), const void *: (const wchar_t *)wcsrchr((s), (c)), void *: wcsrchr((s), (c)))
#define wcsstr(s, c) _Generic((1? (s) : (void *)0), const void *: (const wchar_t *)wcsstr((s), (c)), void *: wcsstr((s), (c)))
#endif

#ifdef _POSIX_C_SOURCE
size_t mbsnrtowcs(wchar_t *__restrict, const char **__restrict, size_t, size_t, mbstate_t *__restrict);
FILE *open_wmemstream(wchar_t **, size_t *);
wchar_t *wcpcpy(wchar_t *__restrict, const wchar_t *__restrict);
wchar_t *wcpncpy(wchar_t *__restrict, const wchar_t *__restrict, size_t);
int wcscasecmp(const wchar_t *, const wchar_t *);
int wcscasecmp_l(const wchar_t *, const wchar_t *, locale_t);
int wcscoll_l(const wchar_t *, const wchar_t *, locale_t);
wchar_t *wcsdup(const wchar_t *);
size_t wcslcat(wchar_t *__restrict, const wchar_t *__restrict, size_t);
size_t wcslcpy(wchar_t *__restrict, const wchar_t *__restrict, size_t);
int wcsncasecmp(const wchar_t *, const wchar_t *, size_t);
int wcsncasecmp_l(const wchar_t *, const wchar_t *, size_t, locale_t);
size_t wcsnlen(const wchar_t *, size_t);
size_t wcsnrtombs(char *__restrict, const wchar_t **__restrict, size_t, size_t, mbstate_t *__restrict);
size_t wcsxfrm_l(wchar_t *__restrict, const wchar_t *__restrict, size_t, locale_t);
#endif

#ifdef _XOPEN_SOURCE
int wcswidth(const wchar_t *, size_t);
int wcwidth(wchar_t);
#endif

#ifdef __cplusplus
}
#endif
#endif
