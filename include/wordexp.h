#ifndef __WORDEXP_H
#define __WORDEXP_H

#define __NEED_size_t
#include <alltypes.h>
#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    size_t we_wordc;
    char **we_wordv;
    size_t we_offs;
} wordexp_t;

enum {
    WRDE_APPEND = 1,
    WRDE_DOOFFS = 2,
    WRDE_NOCMD = 4,
    WRDE_REUSE = 8,
    WRDE_SHOWERR = 16,
    WRDE_UNDEF = 32,
};

enum {
    WRDE_BADCHAR = -10,
    WRDE_BADVAL,
    WRDE_CMDSUB,
    WRDE_NOSPACE,
    WRDE_SYNTAX,
};

int wordexp(const char *__restrict, wordexp_t *__restrict, int);
void wordfree(wordexp_t *);

#ifdef __cplusplus
}
#endif
#endif
