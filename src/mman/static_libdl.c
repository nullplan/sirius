#include <dlfcn.h>
#include <pthread.h>

static void set_dlerr(void)
{
    __pthread_self()->dlerr_msg = "Dynamic loading not supported in static linking.";
}

static int __dladdr(const void *restrict dso, Dl_info_t *restrict dli)
{
    set_dlerr();
    return 0;
}
weak_alias(dladdr, __dladdr);

static int __dlclose(void *dso)
{
    set_dlerr();
    return -1;
}
weak_alias(dlclose, __dlclose);

static char *__dlerror(void)
{
    pthread_t self = __pthread_self();
    char *rv = self->dlerr_msg;
    self->dlerr_msg = 0;
    return rv;
}
weak_alias(dlerror, __dlerror);

static void *__dlopen(const char *name, int flags)
{
    set_dlerr();
    return 0;

}
weak_alias(dlopen, __dlopen);

static void *__dlsym(void *restrict dso, const char *restrict name)
{
    set_dlerr();
    return 0;
}
weak_alias(dlsym, __dlsym);
