#ifndef __FTW_H
#define __FTW_H

#include <sys/stat.h>

#ifdef __cplusplus
extern "C" {
#endif

struct FTW {
    int base;
    int level;
};

enum {
    FTW_F,
    FTW_D,
    FTW_DNR,
    FTW_DP,
    FTW_NS,
    FTW_SL,
    FTW_SLN,
};

enum {
    FTW_PHYS = 1,
    FTW_MOUNT = 2,
    FTW_XDEV = 4,
    FTW_DEPTH = 8,
    FTW_CHDIR = 16,
};

int nftw(const char *, int (*)(const char *, const struct stat *, int, struct FTW *), int, int);

#ifdef __cplusplus
}
#endif
#endif
