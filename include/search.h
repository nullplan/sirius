#ifndef __SEARCH_H
#define __SEARCH_H

#define __NEED_size_t
#include <alltypes.h>
#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct entry {
    char *key;
    void *data;
} ENTRY;

typedef enum { FIND, ENTER } ACTION;
typedef enum { preorder, postorder, endorder, leaf } VISIT;

typedef void posix_tnode;

int hcreate(size_t);
void hdestroy(void);
ENTRY *hsearch(ENTRY, ACTION);

void insque(void *, void *);
void remque(void *);

void *lfind(const void *, const void *, size_t *, size_t, int (*)(const void *, const void *));
void *lsearch(const void *, void *, size_t *, size_t, int (*)(const void *, const void *));

void *tdelete(const void *__restrict, posix_tnode **__restrict, int (*)(const void *, const void *));
posix_tnode *tfind(const void *, posix_tnode *const *, int (*)(const void *, const void *));
posix_tnode *tsearch(const void *, posix_tnode **, int (*)(const void *, const void *));
void twalk(const posix_tnode *, void (*)(const posix_tnode *, VISIT, int));

#ifdef __cplusplus
}
#endif
#endif
