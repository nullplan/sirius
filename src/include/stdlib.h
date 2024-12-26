#ifndef STDLIB_H
#define STDLIB_H

#include "../../include/stdlib.h"

#define ALLOC_ALIGN     (4 * sizeof (size_t))

hidden void *__libc_malloc(size_t);
hidden void *__libc_calloc(size_t, size_t);
#endif
