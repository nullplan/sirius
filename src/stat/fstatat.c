#define _GNU_SOURCE
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <fcntl.h>
#include "libc.h"
#include "syscall.h"

#ifdef SYS_fstatat
#include "kstat_arch.h"

static int fstatat_legacy(int fd, const char *restrict name, struct stat *restrict st, int flags)
{
    struct kstat kst;
    int rv;

    if ((flags & AT_EMPTY_PATH) && fd >= 0 && !*name) {
        rv = __syscall(SYS_fstat, fd, &kst);
        if (rv == -EBADF && __syscall(SYS_fcntl, fd, F_GETFD) >= 0) {
            rv = __syscall(SYS_fstatat, fd, "", &kst, AT_EMPTY_PATH);
            if (rv == -EINVAL) {
                char buf[PROCFD_LEN];
                char *p = __procfdname(fd, buf + sizeof buf);
#ifdef SYS_stat
                rv = __syscall(SYS_stat, p, &kst);
#else
                rv = __syscall(SYS_fstatat, AT_FDCWD, p, &kst, 0);
#endif
            }
        }
    } 
#ifdef SYS_stat
    else if ((fd == AT_FDCWD || *name == '/') && flags == 0)
        rv = __syscall(SYS_stat, name, &kst);
#endif
#ifdef SYS_lstat
    else if ((fd == AT_FDCWD || *name == '/') && flags == AT_SYMLINK_NOFOLLOW)
        rv = __syscall(SYS_lstat, name, &kst);
#endif
    else
        rv = __syscall(SYS_fstatat, fd, name, &kst, flags);

    if (rv == 0) {
        *st = (struct stat) {
            .st_dev = kst.kst_dev,
            .st_ino = kst.kst_ino,
            .st_mode = kst.kst_mode,
            .st_nlink = kst.kst_nlink,
            .st_rdev = kst.kst_rdev,
            .st_uid = kst.kst_uid,
            .st_gid = kst.kst_gid,
            .st_size = kst.kst_size,
            .st_blocks = kst.kst_blocks,
            .st_blksize = kst.kst_blksize,
            .st_atim.tv_sec = kst.kst_atime,
            .st_atim.tv_nsec = kst.kst_atime_nsec,
            .st_mtim.tv_sec = kst.kst_mtime,
            .st_mtim.tv_nsec = kst.kst_mtime_nsec,
            .st_ctim.tv_sec = kst.kst_ctime,
            .st_ctim.tv_nsec = kst.kst_ctime_nsec,
        };
    }
    return rv;
}
#endif

struct statx_timestamp {
    long long   tv_sec;
    unsigned   tv_nsec;
    int   __reserved;
};

struct statx {
    unsigned   stx_mask;
    unsigned   stx_blksize;
    unsigned long long   stx_attributes;
    unsigned   stx_nlink;
    unsigned   stx_uid;
    unsigned   stx_gid;
    unsigned short   stx_mode;
    unsigned short   __spare0[1];
    unsigned long long   stx_ino;
    unsigned long long   stx_size;
    unsigned long long   stx_blocks;
    unsigned long long   stx_attributes_mask;
    struct statx_timestamp  stx_atime;
    struct statx_timestamp  stx_btime;
    struct statx_timestamp  stx_ctime;
    struct statx_timestamp  stx_mtime;
    unsigned   stx_rdev_major;
    unsigned   stx_rdev_minor;
    unsigned   stx_dev_major;
    unsigned   stx_dev_minor;
    unsigned long long   stx_mnt_id;
    unsigned   stx_dio_mem_align;
    unsigned   stx_dio_offset_align;
    unsigned long long   stx_subvol;
    unsigned   stx_atomic_write_unit_min;
    unsigned   stx_atomic_write_unit_max;
    unsigned   stx_atomic_write_segments_max;
    unsigned   __spare1[1];
    unsigned long long   __spare3[9];    /* Spare space for future expansion */
};

#define STATX_BASIC_STATS       0x000007ffU
static int fstatat_statx(int dfd, const char *restrict name, struct stat *restrict st, int flags)
{
    struct statx stx;
    int rv = __syscall(SYS_statx, dfd, name, flags, STATX_BASIC_STATS, &stx);
    if (!rv) {
        *st = (struct stat) {
            .st_dev = makedev(stx.stx_dev_major, stx.stx_dev_minor),
            .st_ino = stx.stx_ino,
            .st_mode = stx.stx_mode,
            .st_nlink = stx.stx_nlink,
            .st_rdev = makedev(stx.stx_rdev_major, stx.stx_rdev_minor),
            .st_uid = stx.stx_uid,
            .st_gid = stx.stx_gid,
            .st_size = stx.stx_size,
            .st_blocks = stx.stx_blocks,
            .st_blksize = stx.stx_blksize,
            .st_atim.tv_sec = stx.stx_atime.tv_sec,
            .st_atim.tv_nsec = stx.stx_atime.tv_nsec,
            .st_mtim.tv_sec = stx.stx_mtime.tv_sec,
            .st_mtim.tv_nsec = stx.stx_mtime.tv_nsec,
            .st_ctim.tv_sec = stx.stx_ctime.tv_sec,
            .st_ctim.tv_nsec = stx.stx_ctime.tv_nsec,
        };
    }
    return rv;
}

hidden int __fstatat(int dfd, const char *restrict name, struct stat *restrict st, int flags)
{
    int rv = -ENOSYS;
#ifdef SYS_fstatat
    if (sizeof ((struct kstat){0}.kst_atime) < sizeof (time_t))
        rv = fstatat_statx(dfd, name, st, flags);
    if (rv == -ENOSYS)
        rv = fstatat_legacy(dfd, name, st, flags);
#else
    rv = fstatat_statx(dfd, name, st, flags);
#endif
    return __syscall_ret(rv);
}
weak_alias(fstatat, __fstatat);
