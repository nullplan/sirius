#ifndef SYS_MMAN_H
#define SYS_MMAN_H

#include "../../../include/sys/mman.h"

extern hidden void *__mmap(void *, size_t, int, int, int, off_t);
extern hidden int __munmap(void *, size_t);
#endif
