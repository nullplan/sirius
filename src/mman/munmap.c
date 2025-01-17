#include <sys/mman.h>
#include "syscall.h"

hidden int __munmap(void *p, size_t sz)
{
    return syscall(SYS_munmap, p, sz);
}
weak_alias(munmap, __munmap);
