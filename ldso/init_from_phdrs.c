#include "libc.h"
#include <pthread.h>
hidden unsigned long __default_stacksize = DEFAULT_STACK_SIZE;
hidden void __init_from_phdrs(const void *ph, size_t num, size_t ent)
{
    /* nothing to do, everything was already done. */
}

