#ifndef SYS_MMAN_H
#define SYS_MMAN_H

#include "../../../include/sys/mman.h"

extern hidden void *__mmap(void *, size_t, int, int, int, off_t);
extern hidden int __munmap(void *, size_t);
extern hidden int __mprotect(void *, size_t, int);
#define SHM_NAMELEN 266
extern hidden int __shm_getname(char *, const char *);
#endif
