#include <sys/mman.h>
#include "syscall.h"

int mprotect(void *p, size_t len, int prot)
{
    return syscall(SYS_mprotect, p, len, prot);
}
