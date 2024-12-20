#ifndef __SYS_UIO_H
#define __SYS_UIO_H

#define __NEED_ssize_t
#define __NEED_size_t
#define __NEED_struct_iovec
#include <alltypes.h>

#ifdef __cplusplus
extern "C" {
#endif
ssize_t readv(int, const struct iovec *, int);
ssize_t writev(int, const struct iovec *, int);
#ifdef __cplusplus
}
#endif
#endif
