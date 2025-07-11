#include <sys/mman.h>
#include "syscall.h"

int posix_madvise(void *addr, size_t len, int adv)
{
    if (adv > 4u) {
        errno = -EINVAL;
        return -1;
    }

    return syscall(SYS_madvise, addr, len, adv);
}
