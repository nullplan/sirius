#ifndef __SYS_MMAN_H
#define __SYS_MMAN_H

#define __NEED_mode_t
#define __NEED_off_t
#define __NEED_size_t
#include <alltypes.h>
#include <features.h>
#include <fcntl.h>
#ifdef __cplusplus
extern "C" {
#endif

#define PROT_NONE   0
#define PROT_READ   0x1
#define PROT_WRITE  0x2
#define PROT_EXEC   0x4
    
#define MAP_SHARED          0x01
#define MAP_PRIVATE         0x02
#define MAP_SHARED_VALIDATE 0x03
#define MAP_DROPPABLE       0x08
#define MAP_FIXED           0x10
#define MAP_ANONYMOUS       0x20
#define MAP_ANON            0x20
#define MAP_POPULATE        0x008000
#define MAP_NONBLOCK        0x010000
#define MAP_STACK           0x020000
#define MAP_HUGETLB         0x040000
#define MAP_SYNC            0x080000
#define MAP_FIXED_NOREPLACE 0x100000

#define MS_ASYNC        1
#define MS_INVALIDATE   2
#define MS_SYNC         4

#define POSIX_MADV_NORMAL           0
#define POSIX_MADV_RANDOM           1
#define POSIX_MADV_SEQUENTIAL       2
#define POSIX_MADV_WILLNEED         3
#define POSIX_MADV_DONTNEED         4

#define MCL_CURRENT     1
#define MCL_FUTURE      2

#define MAP_FAILED  ((void *)-1)

int mlock(const void *, size_t);
int mlockall(int);
void *mmap(void *, size_t, int, int, int, off_t);
int mprotect(void *, size_t, int);
int msync(void *, size_t, int);
int munlock(const void *, size_t);
int munlockall(void);
int munmap(void *, size_t);
int posix_madvise(void *, size_t, int);
int shm_open(const char *, int, mode_t);
int shm_unlink(const char *);
#ifdef __cplusplus
}
#endif
#endif
