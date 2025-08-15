#include <stdlib.h>
#include <sys/mman.h>
#include <syscall.h>
#include <pthread.h>
#include <assert.h>
#include <string.h>
#include "cpu.h"
#include "libc.h"

struct chunk {
    size_t psize;
    size_t csize;
    struct chunk *next;
    struct chunk *prev;
};

static struct lock malloc_lock;
static struct chunk *bins[64];
static uint64_t binmap;
static void *end_of_heap;
#define C_INUSE     1
#define OVERHEAD    (2 * sizeof (size_t))
#define MMAP_THRESH (0x1c00 * ALLOC_ALIGN)

static struct chunk *next_chunk(struct chunk *c) {
    return (struct chunk *)((char *)c + (c->csize & ~C_INUSE));
}

static struct chunk *prev_chunk(struct chunk *c) {
    return (struct chunk *)((char *)c - (c->psize & ~C_INUSE));
}

static struct chunk *chunk_from_mem(void *p) {
    return (struct chunk *)((char *)p - OVERHEAD);
}

static void *mem_from_chunk(struct chunk *c) {
    return (char *)c + OVERHEAD;
}

#define LINE(x) x, x * 5 / 4, x * 6 / 4, x * 7 / 4
static size_t high_bin_low_limits[] = {
    LINE(33), LINE(64), LINE(128), LINE(256),
    LINE(512), LINE(1024), LINE(2048), LINE(4096),
};

/* given chunk size x, return appropriate bin index. */
static size_t bin_index(size_t x) {
    x /= ALLOC_ALIGN;
    if (x <= 32) return x - 1;
    for (size_t i = 0; i < 31; i++)
        if (high_bin_low_limits[i] > x)
            return i + 31;
    return 63;
}

/* given allocation size x, return bin index for chunks that are definitely big enough for it */
static size_t bin_index_up(size_t x) {
    assert(x < MMAP_THRESH);
    x /= ALLOC_ALIGN;
    if (x <= 32) return x - 1;
    for (size_t i = 0; i < 31; i++)
        if (high_bin_low_limits[i] >= x)
            return i + 32;
    abort();
}

static void bin_chunk(struct chunk *c)
{
    assert(c->csize & C_INUSE);
    struct chunk *n = next_chunk(c);
    size_t i = bin_index(c->csize);
    c->csize &= ~C_INUSE;
    n->psize = c->csize;
    c->next = bins[i];
    c->prev = 0;
    if (c->next) c->next->prev = c;
    bins[i] = c;
}

static void unbin_chunk(struct chunk *c, size_t i)
{
    assert(!(c->csize & C_INUSE));
    struct chunk *n = next_chunk(c);
    c->csize |= C_INUSE;
    n->psize = c->csize;
    if (c->next) c->next->prev = c->prev;
    if (c->prev) c->prev->next = c->next;
    else {
        bins[i] = c->next;
        if (!c->next) binmap &= 1ull << i;
    }
}

static struct chunk *split_chunk(struct chunk *c, size_t x)
{
    assert(!(x % ALLOC_ALIGN));
    assert(c->csize & C_INUSE);
    struct chunk *n = next_chunk(c);
    struct chunk *s = (void *)((char *)c + x);
    c->csize = s->psize = x | C_INUSE;
    s->csize = n->psize = ((char *)n - (char *)s) | C_INUSE;
    return s;
}

static struct chunk *expand_heap(size_t x)
{
    size_t x_pa = (x + OVERHEAD + PAGE_SIZE - 1) & -PAGE_SIZE;
    void *p = __mmap(end_of_heap, x_pa, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (p == MAP_FAILED && end_of_heap)
        p = __mmap(0, x_pa, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (p == MAP_FAILED) return 0;
    struct chunk *c, *n;
    if (p == end_of_heap)
        c = chunk_from_mem(end_of_heap);
    else {
        c = chunk_from_mem((char *)p + ALLOC_ALIGN);
        c->psize = C_INUSE;
    }
    n = chunk_from_mem((char *)p + x_pa);
    c->csize = ((char *)n - (char *)c) | C_INUSE;
    if (p == end_of_heap) {
        while (!(c->psize & C_INUSE)) {
            struct chunk *p = prev_chunk(c);
            unbin_chunk(p, bin_index(p->csize));
            p->csize += c->csize - C_INUSE;
            c = p;
        }
    }
    n->psize = c->csize;
    n->csize = C_INUSE;
    end_of_heap = mem_from_chunk(n);
    return c;
}

hidden void *__libc_malloc_impl(size_t x)
{
    /* assumption: x == 0 and x > PTRDIFF_MAX already filtered */
    x = (x + OVERHEAD + ALLOC_ALIGN - 1) & -ALLOC_ALIGN;
    if (x > PTRDIFF_MAX) {
        errno = ENOMEM;
        return 0;
    }
    if (x >= MMAP_THRESH) {
        size_t x_pa = (x + OVERHEAD + PAGE_SIZE - 1) & -PAGE_SIZE;
        void *p = __mmap(0, x_pa, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (p == MAP_FAILED) return 0;
        struct chunk *c = chunk_from_mem((char *)p + ALLOC_ALIGN);
        c->psize = OVERHEAD;
        c->csize = x_pa - OVERHEAD;
        return mem_from_chunk(c);
    }

    struct chunk *c;
    size_t idx = bin_index_up(x);
    __lock(&malloc_lock);
    uint64_t mask = (-1ull << idx) & binmap;
    if (!mask)
        c = expand_heap(x);
    else {
        idx = a_ctz_64(mask);
        c = bins[idx];
        unbin_chunk(c, idx);
    }
    if (c && (c->csize & ~C_INUSE) > x) {
        bin_chunk(split_chunk(c, x));
    }
    __unlock(&malloc_lock);
    return c? mem_from_chunk(c) : 0;
}

hidden void *__libc_calloc_impl(size_t a, size_t b)
{
    a *= b;
    void *p = __libc_malloc(a);
    if (p) {
        struct chunk *c = chunk_from_mem(p);
        if (c->csize & C_INUSE) memset(p, 0, a);
    }
    return p;
}

void __libc_free(void *p)
{
    if (!p) return;
    struct chunk *c = chunk_from_mem(p);
    if (!(c->csize & C_INUSE)) {
        if ((c->csize + c->psize) & (PAGE_SIZE - 1)) a_crash();
        __munmap((char *)c - c->psize, c->csize + c->psize);
        return;
    }
    struct chunk *n = next_chunk(c);
    if (c->csize != n->psize) a_crash();
    __lock(&malloc_lock);
    while (!(n->csize & C_INUSE)) {
        unbin_chunk(n, bin_index(n->csize));
        c->csize += n->csize - C_INUSE;
        n = next_chunk(n);
    }
    while (!(c->psize & C_INUSE)) {
        struct chunk *p = prev_chunk(c);
        unbin_chunk(p, bin_index(p->csize));
        p->csize += c->csize - C_INUSE;
        n->psize = p->csize;
        c = p;
    }
    bin_chunk(c);
    __unlock(&malloc_lock);
}

#define MREMAP_MAYMOVE  1

void *__libc_realloc(void *p, size_t new)
{
    if (!p) return malloc(new);
    if (!new) {
        errno = EINVAL;
        return 0;
    }
    if (new >= PTRDIFF_MAX - OVERHEAD) {
        errno = ENOMEM;
        return 0;
    }
    struct chunk *c = chunk_from_mem(p);
    new = (new + OVERHEAD + ALLOC_ALIGN - 1) & -ALLOC_ALIGN;
    /* possible situations:
     * - old chunk managed, new size managed, shrinking: split old chunk
     * - old chunk managed, new size managed, growing: alloc+copy
     * - old chunk mmapped, new size managed: alloc + copy
     * - old chunk mmapped, new size mmapped: mremap
     */
    if ((c->csize & ~C_INUSE) == new) return p;
    if (!(c->csize & C_INUSE) && new >= MMAP_THRESH) {
        size_t off = c->psize;
        size_t alloc = (off + new + PAGE_SIZE - 1) & -PAGE_SIZE;
        void *np = (void *)syscall(SYS_mremap, (uintptr_t)c - c->psize, c->psize + c->csize, alloc, MREMAP_MAYMOVE);
        if (np == MAP_FAILED)
            return 0;
        c = (void *)((char *)np + off);
        c->csize = alloc - off;
        return mem_from_chunk(c);
    }
    if ((c->csize & C_INUSE) && new < c->csize) {
        struct chunk *s = split_chunk(c, new);
        __lock(&malloc_lock);
        bin_chunk(s);
        __unlock(&malloc_lock);
        return p;
    }
    void *np = __libc_malloc(new - OVERHEAD);
    if (!np) return 0;
    memcpy(np, p, MIN(new, c->csize & ~C_INUSE) - OVERHEAD);
    __libc_free(p);
    return np;
}

void *aligned_alloc(size_t a, size_t sz)
{
    if (!a || (a & (a - 1))) {
        errno = EINVAL;
        return 0;
    }
    if (a <= ALLOC_ALIGN) return __libc_malloc(sz);
    size_t asz = sz + a - 1;
    void *p = __libc_malloc(asz);
    if (!p) return 0;
    struct chunk *c = chunk_from_mem(p);
    void *ap = (void *)(((uintptr_t)p + a - 1) & -a);
    if (!(c->csize & C_INUSE)) {
        struct chunk *ac = chunk_from_mem(ap);
        ac->psize = c->psize + ((char *)ac - (char *)c);
        ac->csize = c->csize - ((char *)ac - (char *)c);
        uintptr_t start = (uintptr_t)ac - ac->psize;
        uintptr_t end = (uintptr_t)ac + ac->csize;
        if (ac->psize >= PAGE_SIZE) {
            __munmap((void *)start, (uintptr_t)ac & -PAGE_SIZE);
            ac->psize = (uintptr_t)ac & (PAGE_SIZE - 1);
        }
        if (ac->csize - OVERHEAD - sz >= PAGE_SIZE) {
            uintptr_t pagebrk = ((uintptr_t)ac + OVERHEAD + sz + PAGE_SIZE - 1) & -PAGE_SIZE;
            __munmap((void *)pagebrk, end - pagebrk);
            ac->csize = pagebrk - (uintptr_t)ac;
        }
        return ap;
    }

    if (ap != p) {
        struct chunk *ac = split_chunk(c, (char *)ap - (char *)p);
        __lock(&malloc_lock);
        bin_chunk(c);
        __unlock(&malloc_lock);
        c = ac;
    }
    sz = (sz + OVERHEAD + ALLOC_ALIGN - 1) & -ALLOC_ALIGN;
    if (c->csize - C_INUSE > sz) {
        struct chunk *s = split_chunk(c, sz);
        __lock(&malloc_lock);
        bin_chunk(s);
        __unlock(&malloc_lock);
    }
    return mem_from_chunk(c);

}

hidden void __donate_malloc_memory(void *p, size_t len)
{
    if (len >= 2 * ALLOC_ALIGN)
    {
        size_t *start = (void *)(((uintptr_t)p + OVERHEAD + ALLOC_ALIGN - 1) & -ALLOC_ALIGN);
        size_t *end = (void *)(((uintptr_t)p + len) & -ALLOC_ALIGN);
        if (start < end) {
            struct chunk *c = chunk_from_mem(start);
            struct chunk *n = chunk_from_mem(end);
            c->psize = C_INUSE;
            c->csize = (end - start) | C_INUSE;
            n->psize = c->csize;
            n->csize = C_INUSE;
            __lock(&malloc_lock);
            bin_chunk(c);
            __unlock(&malloc_lock);
        }
    }
}
