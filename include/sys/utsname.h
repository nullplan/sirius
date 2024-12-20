#ifndef __SYS_UTSNAME_H
#define __SYS_UTSNAME_H

#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

#ifdef _GNU_SOURCE
#define __NSG(x)    x
#else
#define __NSG(x)    __##x
#endif

struct utsname {
    char sysname[65];
    char nodename[65];
    char release[65];
    char version[65];
    char machine[65];
    char __NSG(domainname)[65];
};

int uname(struct utsname *);
#ifdef __cplusplus
}
#endif
#endif
