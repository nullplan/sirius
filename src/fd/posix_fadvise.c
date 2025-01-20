#include <fcntl.h>
#include "syscall.h"

int posix_fadvise(int fd, off_t start, off_t len, int adv)
{
#ifdef ARCH_FADVISE_SWAP_ARGS
    return syscall(SYS_fadvise, fd, adv, __SYSCALL_LL_E(start), __SYSCALL_LL_E(len));
#else
    return syscall(SYS_fadvise, fd, __SYSCALL_LL_O(start), __SYSCALL_LL_E(len), adv);
#endif
}
