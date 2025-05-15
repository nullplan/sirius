#define __STARTUP_CODE
#include "libc.h"
#include "cpu.h"
#include "syscall.h"
#include <features.h>
#include <elf.h>
#include <pthread.h>
#include <stdint.h>
#include <stdalign.h>
#include <string.h>
#include <sys/mman.h>

#ifndef __SIXTY_FOUR
typedef Elf32_Phdr Phdr;
#else
typedef Elf64_Phdr Phdr;
#endif

static struct tls_data tls_data;
hidden struct tls_data __get_tls_data(void) { return tls_data; }

static const void *tls_image;
static size_t tls_len;
static size_t tls_size;
static size_t tls_align;

static struct __pthread *static_copy_tls(void *mem)
{
    if (!tls_size)
        return mem;

#ifdef TLS_VARIANT_1
    char *tls = mem;
    pthread_t tp = (void *)(((uintptr_t)tls + tls_size + alignof (struct __pthread) - 1) & -alignof (struct __pthread));
    size_t *dtv = (void *)(tp + 1);
#else
    size_t *dtv = mem;
    pthread_t tp = (void *)(dtv + 2);
    char *tls = (((uintptr_t)(tp + 1) + GAP_ABOVE_TP + tls_align - 1) & -tls_align);
#endif
    memcpy(tls, tls_image, tls_len);
    dtv[1] = (size_t)tls;
    tp->dtv = dtv;
    return tp;
}
weak_alias(__copy_tls, static_copy_tls);

extern weak const size_t _DYNAMIC[];
static struct {
    struct __pthread tp;
    uintptr_t space[20];
} builtin_tls;

static unsigned long defstack = DEFAULT_STACK_SIZE;
weak_alias(__default_stacksize, defstack);

static struct __pthread *init_tp(struct __pthread *tp)
{
    tp->self = tp->next = tp->prev = tp;
    if (__set_thread_area(__tp_adjust(tp)))
        a_crash();
    return tp;
}

static struct __pthread *static_init_from_phdrs(const void *start, size_t phnum, size_t phent)
{
    const Phdr *ph = start;
    const Phdr *ph_tls = 0;
    size_t i;
    size_t base = 0;
    int found_load = 0;
    for (i = 0; i < phnum; i++, ph = (const void *)((const char *)ph + phent))
    {
        if (ph->p_type == PT_GNU_STACK)
        {
            if (ph->p_memsz > __default_stacksize && ph->p_memsz < MAX_DEFAULT_STACK_SIZE)
                __default_stacksize = ph->p_memsz;
        }
        else if (ph->p_type == PT_TLS)
            ph_tls = ph;
        else if (ph->p_type == PT_DYNAMIC && _DYNAMIC)
            base = (uintptr_t)_DYNAMIC - ph->p_vaddr;
        else if (ph->p_type == PT_PHDR)
            base = (uintptr_t)start - ph->p_vaddr;
        else if (ph->p_type == PT_LOAD && !found_load)
        {
            found_load = 1;
            base = ((uintptr_t)start & -PAGE_SIZE) - ph->p_vaddr;
        }
    }

    if (ph_tls) {
        tls_image = (void *)(base + ph_tls->p_vaddr);
        tls_len = ph_tls->p_filesz;
        tls_size = ph_tls->p_memsz;
        tls_align = ph_tls->p_align;
#ifdef TLS_VARIANT_1
        tls_data.size = (tls_size + alignof(struct __pthread) - 1) & -alignof(struct __pthread);
        tls_data.size += sizeof (struct __pthread) + GAP_ABOVE_TP + 2 * sizeof (size_t);
#else
        tls_data.size = (2 * sizeof (size_t) + sizeof (struct __pthread) + GAP_ABOVE_TP + tls_align - 1) & -tls_align;
        tls_data.size += tls_size;
#endif
        tls_data.align = tls_align;
        if (tls_data.align < alignof (struct __pthread))
            tls_data.align = alignof (struct __pthread);
    } else {
        tls_data.size = sizeof (struct __pthread) + GAP_ABOVE_TP;
        tls_data.align = alignof (struct __pthread);
    }
    char *tlsbase;
    size_t need = ((-(uintptr_t)&builtin_tls) & (tls_data.align - 1)) + tls_data.size;
    if (need <= sizeof builtin_tls) {
        tlsbase = (void *)((((uintptr_t)&builtin_tls) + tls_data.align - 1) & -tls_data.align);
    } else {
#ifdef SYS_mmap2
#undef SYS_mmap
#define SYS_mmap SYS_mmap2
#endif
        tlsbase = (void *)__syscall(SYS_mmap, 0, tls_data.size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    }
    return init_tp(static_copy_tls(tlsbase));
}
weak_alias(__init_from_phdrs, static_init_from_phdrs);
