#ifndef STDLIB_H
#define STDLIB_H

#include "../../include/stdlib.h"
#include <pthread.h>

#define ALLOC_ALIGN     (4 * sizeof (size_t))

hidden void *__libc_malloc(size_t);
hidden void *__libc_calloc(size_t, size_t);
hidden void __libc_free(void *);
hidden void *__libc_realloc(void *, size_t);
hidden void *__libc_malloc_impl(size_t);
hidden void *__libc_calloc_impl(size_t, size_t);
hidden char **__lookup_env(const char *, size_t);
hidden void __donate_malloc_memory(void *, size_t);
extern char **__environ;
hidden int __reallocenv(size_t);
hidden void __add_rm_env_map(char *, char *);
hidden void __qsort_r(void *, size_t, size_t, int (*)(const void *, const void *, void *), void *);

#undef MB_CUR_MAX
#define MB_CUR_MAX (__pthread_self()->locale->is_utf8? 4 : 1)
#endif
