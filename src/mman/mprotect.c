#include <sys/mman.h>
#include "syscall.h"

hidden int __mprotect(void *p, size_t len, int prot)
{
    return syscall(SYS_mprotect, p, len, prot);
}
weak_alias(mprotect, __mprotect);
