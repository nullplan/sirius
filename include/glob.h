#ifndef __GLOB_H
#define __GLOB_H

#define __NEED_size_t
#include <alltypes.h>
#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    size_t gl_pathc;
    char **gl_pathv;
    size_t gl_offs;
} glob_t;

#define GLOB_APPEND     1
#define GLOB_DOOFFS     2
#define GLOB_ERR        4
#define GLOB_MARK       8
#define GLOB_NOCHECK    16
#define GLOB_NOESCAPE   32
#define GLOB_NOSORT     64

#define GLOB_ABORTED    (-1)
#define GLOB_NOMATCH    (-2)
#define GLOB_NOSPACE    (-3)

int glob(const char *__restrict, int, int (*)(const char *, int), glob_t *__restrict);
void globfree(glob_t *);

#ifdef __cplusplus
}
#endif
#endif
