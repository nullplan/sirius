#ifndef __SYS_STATVFS_H
#define __SYS_STATVFS_H

#define __NEED_fsblkcnt_t
#define __NEED_fsfilcnt_t
#include <alltypes.h>
#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif


struct statvfs {
    unsigned long f_type;
    unsigned long f_bsize;
    fsblkcnt_t f_blocks;
    fsblkcnt_t f_bfree;
    fsblkcnt_t f_bavail;
    fsfilcnt_t f_files;
    fsfilcnt_t f_free;
    unsigned long f_fsid;
#ifndef __SIXTY_FOUR
    unsigned long __pad0;
#endif
    unsigned long f_namemax;
    unsigned long __pad1;
    unsigned long f_flags;
    unsigned long __spare[2];
    fsfilcnt_t  f_favail;
};

#define ST_RDONLY       0x0001
#define ST_NOSUID       0x0002
#define ST_NODEV        0x0004
#define ST_NOEXEC       0x0008
#define ST_SYNCHRONOUS  0x0010
#define ST_VALID        0x0020
#define ST_MANDLOCK     0x0040
#define ST_NOATIME      0x0400
#define ST_NODIRATIME   0x0800
#define ST_RELATIME     0x1000
#define ST_NOSYMFOLLOW  0x2000

int fstatvfs(int, struct statvfs *);
int statvfs(const char *__restrict, struct statvfs *__restrict);

#ifdef __cplusplus
}
#endif
#endif
