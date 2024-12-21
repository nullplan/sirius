#ifndef __DEVCTL_H
#define __DEVCTL_H

#define __NEED_size_t
#include <alltypes.h>
#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

int posix_devctl(int, int, void *__restrict, size_t, int *__restrict);

#ifdef __cplusplus
}
#endif
#endif
