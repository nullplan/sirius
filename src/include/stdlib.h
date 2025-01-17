#ifndef STDLIB_H
#define STDLIB_H

#include "../../include/stdlib.h"

#define ALLOC_ALIGN     (4 * sizeof (size_t))

hidden void *__libc_malloc(size_t);
hidden void *__libc_calloc(size_t, size_t);
hidden char **__lookup_env(const char *, size_t);
extern hidden char **__environ;
hidden int __reallocenv(size_t);
hidden void __add_rm_env_map(char *, char *);
#endif
