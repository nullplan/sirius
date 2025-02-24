#ifndef __FCNTL_H
#define __FCNTL_H

#define __NEED_pid_t
#define __NEED_off_t
#define __NEED_mode_t
#define __NEED_SEEK_SET
#include <alltypes.h>
#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

struct f_owner_ex {
    int type;
    pid_t pid;
};

struct flock {
    short l_type;
    short l_whence;
    off_t l_start;
    off_t l_len;
    pid_t l_pid;
};

#define F_DUPFD         0
#define F_DUPFD_CLOEXEC 1030
#define F_GETFD         1
#define F_SETFD         2
#define F_GETFL         3
#define F_SETFL         4
#ifdef __SIXTY_FOUR
#define F_GETLK         5
#define F_SETLK         6
#define F_SETLKW        7
#else
#define F_GETLK         12
#define F_SETLK         13
#define F_SETLKW        14
#endif
#define F_SETOWN        8
#define F_GETOWN        9
#define F_SETSIG        10
#define F_GETSIG        11
#define F_SETOWN_EX     15
#define F_GETOWN_EX     16
#define F_GETOWNER_UIDS 17
#define F_OFD_GETLK     36
#define F_OFD_SETLK     37
#define F_OFD_SETLKW    38

#define F_OWNER_TID     0
#define F_OWNER_PID     1
#define F_OWNER_PGRP    2

#define FD_CLOEXEC      1

#define F_RDLCK         0
#define F_WRLCK         1
#define F_UNLCK         2

#define O_ACCMODE       0x00200003
#define O_RDONLY        0x00000000
#define O_WRONLY        0x00000001
#define O_RDWR          0x00000002
#define O_CREAT         0x00000040
#define O_EXCL          0x00000080
#define O_NOCTTY        0x00000100
#define O_TRUNC         0x00000200
#define O_APPEND        0x00000400
#define O_NONBLOCK      0x00000800
#define O_DSYNC         0x00001000
#define O_DIRECT        0x00004000
#define O_LARGEFILE     0x00008000
#define O_DIRECTORY     0x00010000
#define O_NOFOLLOW      0x00020000
#define O_NOATIME       0x00040000
#define O_CLOEXEC       0x00080000
#define __O_SYNC        0x00100000
#define O_PATH          0x00200000
#define __O_TMPFILE     0x00400000

#include <bits/fcntl.h>

#define O_SYNC          (__O_SYNC|O_DSYNC)
#define O_TMPFILE       (__O_TMPFILE|O_DIRECTORY)
#define O_EXEC          O_PATH
#define O_SEARCH        O_PATH
#define O_TTY_INIT      0

#define AT_FDCWD        (-100)
#define AT_SYMLINK_NOFOLLOW 0x100
#define AT_SYMLINK_FOLLOW   0x400
#define AT_NO_AUTOMOUNT     0x800
#define AT_EMPTY_PATH       0x1000
#define AT_STATX_SYNC_TYPE      0x6000
#define AT_STATX_SYNC_AS_STAT   0
#define AT_STATX_FORCE_SYNC     0x2000
#define AT_STATX_DONT_SYNC      0x4000
#define AT_RECURSIVE            0x8000

#define AT_RENAME_NOREPLACE     0x0001
#define AT_RENAME_EXCHANGE      0x0002
#define AT_RENAME_WHITEOUT      0x0004
#define AT_EACCESS              0x200
#define AT_REMOVEDIR            0x200
#define AT_HANDLE_FID           0x200
#define AT_HANDLE_MNT_ID_UNIQUE 0x001

#define POSIX_FADV_NORMAL       0
#define POSIX_FADV_RANDOM       1
#define POSIX_FADV_SEQUENTIAL   2
#define POSIX_FADV_WILLNEED     3
#define POSIX_FADV_DONTNEED     4
#define POSIX_FADV_NOREUSE      5

int creat(const char *, mode_t);
int fcntl(int, int, ...);
int open(const char *, int, ...);
int openat(int, const char *, int, ...);
int posix_fadvise(int, off_t, off_t, int);
int posix_fallocate(int, off_t, off_t);

#ifdef __cplusplus
}
#endif
#endif
