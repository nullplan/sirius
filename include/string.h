#ifndef __STRING_H
#define __STRING_H

#define __STDC_VERSION_STRING_H__ 202311L

#define __NEED_size_t
#define __NEED_NULL
#include <alltypes.h>

#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

void *memcpy(void *__restrict, const void *__restrict, size_t);
void *memccpy(void *__restrinct, const void *__restrict, int, size_t);
void *memmove(void *, const void *, size_t);
char *strcpy(char *__restrict, const char *__restrict);
char *strncpy(char *__restrict, const char *__restrict, size_t);
char *strdup(const char *);
char *strndup(const char *, size_t);
char *strcat(char *__restrict, const char *__restrict);
char *strncat(char *__restrict, const char *__restrict, size_t);
int memcmp(const void *, const void *, size_t);
int strcmp(const char *, const char *);
int strcoll(const char *, const char *);
int strncmp(const char *, const char *, size_t);
size_t strxfrm(char *__restrict, const char *__restrict, size_t);
void *memchr(const void *, int, size_t);
char *strchr(const char *, int);
size_t strcspn(const char *, const char *);
char *strpbrk(const char *, const char *);
char *strrchr(const char *, int);
size_t strspn(const char *, const char *);
char *strtok(char *__restrict, const char *__restrict);
void *memset(void *, int, size_t);
void *memset_explicit(void *, int, size_t);
char *strerror(int);
size_t strlen(const char *);

#if __STDC_VERSION__ >= 202311L
#define memchr(s, c, n) _Generic((1? (s) : (void *)0), const void *: (const void *)memchr((s), (c), (n)), void *: memchr((s), (c), (n)))
#define strchr(s, c) _Generic((1? (s) : (void *)0), const void *: (const char *)strchr((s), (c)), void *: strchr((s), (c)))
#define strpbrk(s, c) _Generic((1? (s) : (void *)0), const void *: (const char *)strpbrk((s), (c)), void *: strpbrk((s), (c)))
#define strrchr(s, c) _Generic((1? (s) : (void *)0), const void *: (const char *)strrchr((s), (c)), void *: strrchr((s), (c)))
#define strstr(s, c) _Generic((1? (s) : (void *)0), const void *: (const char *)strstr((s), (c)), void *: strstr((s), (c)))
#endif
#ifdef __cplusplus
}
#endif
#endif
