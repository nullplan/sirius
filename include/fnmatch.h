#ifndef __FNMATCH_H
#define __FNMATCH_H

#ifdef __cplusplus
extern "C" {
#endif

#define FNM_NOMATCH     1
#define FNM_PATHNAME    1
#define FNM_PERIOD      2
#define FNM_NOESCAPE    4
#define FNM_CASEFOLD    8
#define FNM_IGNORECASE  8

int fnmatch(const char *, const char *, int);

#ifdef __cplusplus
}
#endif
#endif
