#ifndef __GRP_H
#define __GRP_H

#define __NEED_gid_t
#define __NEED_size_t
#include <alltypes.h>
#include <features.h>

#ifdef __cplusplus
extern "C" {
#endif

struct group {
    char *gr_name;
    gid_t gr_gid;
    char **gr_mem;
};


struct group *getgrgid(gid_t);
struct group *getgrnam(const char *);
int getgrgid_r(gid_t, struct group *, char *, size_t, struct group **);
int getgrnam_r(const char *, struct group *, char *, size_t, struct group **);

#ifdef _XOPEN_SOURCE
void setgrent(void);
struct group *getgrent(void);
void endgrent(void);
#endif

#ifdef __cplusplus
}
#endif
#endif
