#ifndef VDSO_H
#define VDSO_H

#include <features.h>
hidden void __init_vdso(const void *p);
hidden void *__vdsosym(const char *name, const char *ver);
#endif
