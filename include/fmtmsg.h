#ifndef __FMTMSG_H
#define __FMTMSG_H

#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

enum {
    MM_HARD,
    MM_SOFT,
    MM_FIRM,
    MM_APPL,
    MM_UTIL,
    MM_OPSYS,
    MM_RECOVER,
    MM_NRECOV,
    MM_HALT,
    MM_ERROR,
    MM_WARNING,
    MM_INFO,
    MM_NOSEV,
    MM_PRINT,
    MM_CONSOLE,
    MM_OK,
    MM_NOTOK,
    MM_NOMSG,
    MM_NOCON,
};

#define MM_NULLLBL ((char *)0)
#define MM_NULLSEV  0
#define MM_NULLMC   0L
#define MM_NULLTXT  ((char *)0)
#define MM_NULLACT  ((char *)0)
#define MM_NULLTAG  ((char *)0)

int fmtmsg(long, const char *, int, const char *, const char *, const char *);

#ifdef __cplusplus
}
#endif
#endif
