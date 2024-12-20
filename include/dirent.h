#ifndef __DIRENT_H
#define __DIRENT_H

#define __NEED_ino_t
#define __NEED_reclen_t
#define __NEED_size_t
#define __NEED_ssize_t
#include <alltypes.h>
#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct __dirstream DIR;

struct dirent {
    ino_t d_ino;
    long long d_off;
    reclen_t d_reclen;
    unsigned char d_type;
    char d_name[];
};

struct posix_dent {
    ino_t d_ino;
    long long d_off;
    reclen_t d_reclen;
    unsigned char d_type;
    char d_name[];
};

#define DT_UNKNOWN      0
#define DT_FIFO         1
#define DT_CHR          2
#define DT_DIR          4
#define DT_BLK          6
#define DT_REG          8
#define DT_LNK          10
#define DT_SOCK         12
#define DT_WHT          14


int alphasort(const struct dirent **, const struct dirent **);
int closedir(DIR *);
int dirfd(DIR *);
DIR *fdopendir(int);
DIR *opendir(const char *);
ssize_t posix_getdents(int, void *, size_t, int);
struct dirent *readdir(DIR *);
int readdir_r(DIR *__restrict, struct dirent *__restrict, struct dirent **__restrict);
void rewinddir(DIR *);
int scandir(const char *, struct dirent ***, int (*)(const struct dirent *), int (*)(const struct dirent **, const struct dirent **));

#ifdef _XOPEN_SOURCE
void seekdir(DIR *, long);
long telldir(DIR *);
#endif


#ifdef __cplusplus
}
#endif
#endif
