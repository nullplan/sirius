#ifndef __SYS_STAT_H
#define __SYS_STAT_H

#include <features.h>
#define __NEED_dev_t
#define __NEED_ino_t
#define __NEED_mode_t
#define __NEED_nlink_t
#define __NEED_uid_t
#define __NEED_gid_t
#define __NEED_time_t
#define __NEED_struct_timespec
#define __NEED_off_t
#ifdef _XOPEN_SOURCE
#define __NEED_blkcnt_t
#define __NEED_blksize_t
#endif
#include <alltypes.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _XOPEN_SOURCE
#define __NSX(x)    x
#else
#define __NSX(x)    __##x
#endif

struct stat {
    dev_t st_dev;
    ino_t st_ino;
    mode_t st_mode;
    nlink_t st_nlink;
    dev_t __NSX(st_rdev);
    uid_t st_uid;
    gid_t st_gid;
    off_t st_size;
    unsigned long long __NSX(st_blocks);
    unsigned __NSX(st_blksize);
    struct timespec st_atim;
    struct timespec st_mtim;
    struct timespec st_ctim;
};

#define st_atime    st_atim.tv_sec
#define st_mtime    st_mtim.tv_sec
#define st_ctime    st_ctim.tv_sec

#define S_IFMT      0170000
#define S_IFSOCK    0140000
#define S_IFLNK     0120000
#define S_IFREG     0100000
#define S_IFBLK     0060000
#define S_IFDIR     0040000
#define S_IFCHR     0020000
#define S_IFIFO     0010000
#define S_ISUID     0004000
#define S_ISGID     0002000
#define S_ISVTX     0001000
#define S_IRWXU     0000700
#define S_IRUSR     0000400
#define S_IWUSR     0000200
#define S_IXUSR     0000100
#define S_IRWXG     0000070
#define S_IRGRP     0000040
#define S_IWGRP     0000020
#define S_IXGRP     0000010
#define S_IRWXO     0000007
#define S_IROTH     0000004
#define S_IWOTH     0000002
#define S_IXOTH     0000001

#define S_ISSOCK(m) (((m) & S_IFMT) == S_IFSOCK)
#define S_ISLNK(m)  (((m) & S_IFMT) == S_IFLNK)
#define S_ISREG(m)  (((m) & S_IFMT) == S_IFREG)
#define S_ISBLK(m)  (((m) & S_IFMT) == S_IFBLK)
#define S_ISDIR(m)  (((m) & S_IFMT) == S_IFDIR)
#define S_ISCHR(m)  (((m) & S_IFMT) == S_IFCHR)
#define S_ISFIFO(m) (((m) & S_IFMT) == S_IFIFO)
#define S_TYPEISMQ(x)   0
#define S_TYPEISSEM(x)   0
#define S_TYPEISSHM(x)   0

#define UTIME_NOW   ((1l << 30) - 1)
#define UTIME_OMIT  ((1l << 30) - 2)

int chmod(const char *, mode_t);
int fchmod(int, mode_t);
int fchmodat(int, const char *, mode_t, int);
int fstat(int, struct stat *);
int fstatat(int, const char *__restrict, struct stat *__restrict, int);
int futimens(int, const struct timespec [2]);
int lstat(const char *__restrict, struct stat *__restrict);
int mkdir(const char *, mode_t);
int mkdirat(int, const char *, mode_t);
int mkfifo(const char *, mode_t);
int mkfifoat(int, const char *, mode_t);
int stat(const char *__restrict, struct stat *__restrict);
mode_t umask(mode_t);
int utimensat(int, const char *, const struct timespec [2], int);

#ifdef _XOPEN_SOURCE
int mknod(const char *, mode_t, dev_t);
int mknodat(int, const char *, mode_t, dev_t);
#endif
#ifdef __cplusplus
}
#endif
#endif
