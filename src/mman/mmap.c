#include <sys/mman.h>
#include "syscall.h"

hidden void *__mmap(void *b, size_t len, int prot, int flags, int fd, off_t off)
{
    #ifdef SYS_mmap2
    /* on 32-bit architectures, Linux often provides SYS_mmap2,
     * which takes off as 32-bit number, but numbering some unit
     * of bytes.
     *
     * That unit is usually 4kB, even on archs that don't have 4kB
     * pages (or might have other page sizes). But some archs might differ.
     *
     * I need to detect the case where any bits are set that are discarded
     * by the conversion to syscall_arg_t in the syscall macro call.
     * So the low 12 bits must be clear, but also the high 20 bits.
     *
     * 20 is the difference in bits between the size of syscall_arg_t and
     * off_t, if off_t is also shifted by 12 bits.
     */
    #ifndef ARCH_MMAP2_UNIT
    #define ARCH_MMAP2_UNIT 4096
    #endif
    off_t reject = ((-1ull << (sizeof (syscall_arg_t) * 8 - 1) << 1) * ARCH_MMAP2_UNIT) | (ARCH_MMAP2_UNIT - 1);
    if (off & reject) {
        errno = EINVAL;
        return MAP_FAILED;
    }
    void *p = (void *)syscall(SYS_mmap2, b, len, prot, flags, fd, off >> ARCH_MMAP2_UNIT_BITS);
    #else
    void *p = (void *)syscall(SYS_mmap, b, len, prot, flags, fd, off);
    #endif
    return p;
}
weak_alias(mmap, __mmap);
