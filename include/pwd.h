#ifndef __PWD_H
#define __PWD_H

#define __NEED_size_t
#define __NEED_uid_t
#define __NEED_gid_t
#include <alltypes.h>
#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

struct passwd {
    char *pw_name;
    uid_t pw_uid;
    gid_t pw_gid;
    char *pw_dir;
    char *pw_shell;
};

struct passwd *getpwnam(const char *);
struct passwd *getpwuid(uid_t);
int getpwnam_r(const char *, struct passwd *, char *, size_t, struct passwd **);
int getpwuid_r(uid_t, struct passwd *, char *, size_t, struct passwd **);

#ifdef _XOPEN_SOURCE
void setpwent(void);
struct passwd *getpwent(void);
void endpwent(void);
#endif

#ifdef __cplusplus
}
#endif
#endif
