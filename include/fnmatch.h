#ifndef __FNMATCH_H
#define __FNMATCH_H

#ifdef __cplusplus
extern "C" {
#endif

enum {
    FNM_NOMATCH,
    FNM_PATHNAME,
    FNM_PERIOD,
    FNM_NOESCAPE,
    FNM_CASEFOLD,
    FNM_IGNORECASE,
};

int fnmatch(const char *, const char *, int);

#ifdef __cplusplus
}
#endif
#endif
