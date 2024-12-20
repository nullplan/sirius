#ifndef __DLFCN_H
#define __DLFCN_H

#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const char *dli_fname;
    void *dli_fbase;
    const char *dli_sname;
    void *dli_saddr;
} Dl_info_t;

#define RTLD_LAZY   1
#define RTLD_NOW    0
#define RTLD_GLOBAL 0
#define RTLD_LOCAL  2

int dladdr(const void *__restrict, Dl_info_t *__restrict);
int dlclose(void *);
char *dlerror(void);
void *dlopen(const char *, int);
void *dlsym(void *__restrict, const char *__restrict);

#ifdef __cplusplus
}
#endif
#endif
