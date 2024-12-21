#ifndef __ICONV_H
#define __ICONV_H

#define __NEED_size_t
#include <alltypes.h>
#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned long iconv_t;

size_t iconv(iconv_t, char **__restrict, size_t *__restrict, char *__restrict, size_t *__restrict);
int iconv_close(iconv_t);
iconv_t iconv_open(const char *, const char *);

#ifdef __cplusplus
}
#endif
#endif
