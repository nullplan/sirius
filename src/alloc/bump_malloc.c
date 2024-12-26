#include <stdlib.h>
#include <pthread.h>
#include <syscall.h>
#include <stdint.h>
#include <sys/mman.h>
#include "libc.h"

static struct lock bump_lock;
static uintptr_t heap, hwm;

#ifdef SYS_mmap2
#undef SYS_mmap
#define SYS_mmap SYS_mmap2
#endif

static void *bump_malloc(size_t x)
{
    x = (x + ALLOC_ALIGN - 1) & -ALLOC_ALIGN;
    __lock(&bump_lock);
    if (heap + x < x || heap + x > hwm) {
        size_t x_pa = (x + PAGE_SIZE - 1) & -PAGE_SIZE;
        void *p = (void *)syscall(SYS_mmap, hwm, x_pa, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (p == MAP_FAILED && hwm)
            p = (void *)syscall(SYS_mmap, 0, x_pa, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (p == MAP_FAILED) {
            __unlock(&bump_lock);
            return 0;
        }
        if ((uintptr_t)p != hwm) {
            heap = (uintptr_t)p;
            hwm = heap + x_pa;
        } else {
            hwm += x_pa;
        }
    }
    void *r = (void *)heap;
    heap += x;
    __unlock(&bump_lock);
    return r;
}
weak_alias(__libc_malloc, bump_malloc);

static void *bump_calloc(size_t a, size_t b)
{
    return bump_malloc(a * b);
}
weak_alias(__libc_calloc, bump_calloc);

void *malloc(size_t x)
{
    if (!x) {
        errno = EINVAL;
        return 0;
    }
    if (x > PTRDIFF_MAX) {
        errno = ENOMEM;
        return 0;
    }
    return __libc_malloc(x);
}

void *calloc(size_t a, size_t b)
{
    if (a > PTRDIFF_MAX/b) {
        errno = ENOMEM;
        return 0;
    }
    if (!a || !b) {
        errno = EINVAL;
        return 0;
    }
    return __libc_calloc(a, b);
}
