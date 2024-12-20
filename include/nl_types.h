#ifndef __NL_TYPES_H
#define __NL_TYPES_H

#define __NEED_nl_item
#include <alltypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __catalogue *nl_catd;
#define NL_SETD     (-1)
#define NL_CAT_LOCALE   1

int catclose(nl_catd);
char *catgets(nl_catd, int, int, const char *);
nl_catd catopen(const char *, int);

#ifdef __cplusplus
}
#endif
#endif
