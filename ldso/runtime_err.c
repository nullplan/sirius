#include "ldso.h"
#include <pthread.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

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
        free(self->dlerr_alloc);
        self->dlerr_alloc = malloc(len + 1);
        if (self->dlerr_alloc)
        {
            self->dlerr_alloc_size = len + 1;
            va_start(ap, fmt);
            vsnprintf(self->dlerr_alloc, len + 1, fmt, ap);
            va_end(ap);
        }
    }
    self->dlerr_msg = self->dlerr_alloc;
    return -1;
}
