#include "ldso.h"
#include <pthread.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

hidden void __free_dlerror(void)
{
    __libc_free(__pthread_self()->dlerr_alloc);
    __pthread_self()->dlerr_alloc = 0;
    __pthread_self()->dlerr_alloc_size = 0;
}

hidden int __dl_runtime_err(const char *fmt, ...)
{
    pthread_t self = __pthread_self();
    self->dlerr_msg = 0;

    va_list ap;
    va_start(ap, fmt);
    int len = vsnprintf(self->dlerr_alloc, self->dlerr_alloc_size, fmt, ap);
    va_end(ap);

    if (len >= self->dlerr_alloc_size)
    {
        __libc_free(self->dlerr_alloc);
        self->dlerr_alloc = __libc_malloc(len + 1);
        if (self->dlerr_alloc)
        {
            self->dlerr_alloc_size = len + 1;
            va_start(ap, fmt);
            vsnprintf(self->dlerr_alloc, len + 1, fmt, ap);
            va_end(ap);
        } else self->dlerr_alloc_size = 0;
    }
    self->dlerr_msg = self->dlerr_alloc;
    return -1;
}
