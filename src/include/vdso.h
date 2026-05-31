#ifndef VDSO_H
#define VDSO_H

#include <features.h>
#include <stddef.h>
#include <stdint.h>
hidden void __init_vdso(const void *p);
hidden void *__vdsosym(const char *name, const char *ver);
hidden size_t __count_ghashtab(const uint32_t *gh);
#endif
