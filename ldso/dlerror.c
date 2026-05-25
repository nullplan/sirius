#include <dlfcn.h>
#include <pthread.h>

char *dlerror(void)
{
    pthread_t self = __pthread_self();
    char *rv = self->dlerr_msg;
    self->dlerr_msg = 0;
    return rv;
}
