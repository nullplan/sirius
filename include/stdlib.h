#ifndef __STDLIB_H
#define __STDLIB_H

#define __STDC_VERSION_STDLIB_H__ 202311L
#define __NEED_size_t
#define __NEED_wchar_t
#define __NEED_once_flag
#define __NEED_NULL
#include <alltypes.h>
#include <features.h>
#ifdef _POSIX_C_SOURCE
#include <fcntl.h>
#include <sys/wait.h>
#endif
#ifdef __cplusplus
extern "C" {
#endif

typedef struct { int quot, rem; } div_t;
typedef struct { long quot, rem; } ldiv_t;
typedef struct { long long quot, rem; } lldiv_t;

#define EXIT_FAILURE    1
#define EXIT_SUCCESS    0
#define RAND_MAX        0x7fffffff
int __mb_cur_max(void);
#define MB_CUR_MAX      (__mb_cur_max())

void call_once(once_flag *, void (*)(void));
double atof(const char *);
int atoi(const char *);
long atol(const char *);
long long atoll(const char *);
int strfromd(char *__restrict, size_t, const char *__restrict, double);
int strfromf(char *__restrict, size_t, const char *__restrict, float);
int strfroml(char *__restrict, size_t, const char *__restrict, long double);
double strtod(const char *__restrict, char **__restrict);
float strtof(const char *__restrict, char **__restrict);
long double strtold(const char *__restrict, char **__restrict);
long strtol(const char *__restrict, char **__restrict, int);
long long strtoll(const char *__restrict, char **__restrict, int);
unsigned long strtoul(const char *__restrict, char **__restrict, int);
unsigned long long strtoull(const char *__restrict, char **__restrict, int);
int rand(void);
void srand(unsigned);
void *aligned_alloc(size_t, size_t);
void *calloc(size_t, size_t);
void free(void *);
void free_sized(void *, size_t);
void free_aligned_sized(void *, size_t, size_t);
void *malloc(size_t);
void *realloc(void *, size_t);
_Noreturn void abort(void);
int atexit(void (*)(void));
_Noreturn void exit(int);
_Noreturn void _Exit(int);
int at_quick_exit(void (*)(void));
_Noreturn void quick_exit(int);
char *getenv(const char *);
int system(const char *);
void *bsearch(const void *, const void *, size_t, size_t, int (*)(const void *, const void *));
void qsort(void *, size_t, size_t, int (*)(const void *, const void *));
int abs(int);
long labs(long);
long long llabs(long long);
div_t div(int, int);
ldiv_t ldiv(long, long);
lldiv_t lldiv(long long, long long);
int mblen(const char *, size_t);
int mbtowc(wchar_t *__restrict, const char *__restrict, size_t);
int wctomb(char *, wchar_t);
size_t mbstowcs(wchar_t *__restrict, const char *__restrict, size_t);
size_t wcstombs(char *__restrict, const wchar_t *__restrict, size_t);
size_t memalignment(const void *);

#if __STDC_VERSION__ >= 202311L
#define bsearch(k, b, n, sz, c) _Generic((1? (b) : (void *)0), const void *: (const void *)bsearch((k), (b), (n), (sz), (c)), void *: bsearch((k), (b), (n), (sz), (c)))
#endif

#ifdef _POSIX_C_SOURCE
int getsubopt(char **__restrict, char *const *__restrict, char **__restrict);
char *mkdtemp(char *);
int mkostemp(char *, int);
int mkstemp(char *);
int posix_memalign(void **, size_t, size_t);
void qsort_r(void *, size_t, size_t, int (*)(const void *, const void *, void *), void *);
void *reallocarray(void *, size_t, size_t);
char *realpath(const char *__restrict, char *__restrict);
char *secure_getenv(const char *);
int setenv(const char *, const char *, int);
int unsetenv(const char *);
#endif

#ifdef _XOPEN_SOURCE
long a64l(const char *);
char *l64a(long);

double drand48(void);
double erand48(unsigned short [3]);
char *initstate(unsigned, char *, size_t);
long jrand48(unsigned short [3]);
void lcong48(unsigned short [7]);
long lrand48(void);
long mrand48(void);
long nrand48(unsigned short [3]);
unsigned short *seed48(unsigned short [3]);
void srand48(long);
long random(void);
void srandom(unsigned);

int grantpt(int);
int unlockpt(int);
int posix_openpt(int);
char *ptsname(int);
int ptsname_r(int, char *, size_t);

int putenv(char *);
#endif

#ifdef __cplusplus
}
#endif
#endif
