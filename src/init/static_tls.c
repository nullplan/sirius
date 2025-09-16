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

static struct tls_module tls_mod;

extern weak const size_t _DYNAMIC[];
static struct {
    struct __pthread tp;
    uintptr_t space[20];
} builtin_tls;

static unsigned long defstack = DEFAULT_STACK_SIZE;
weak_alias(__default_stacksize, defstack);

static void static_init_from_phdrs(const void *start, size_t phnum, size_t phent, size_t hwcap, size_t sysinfo)
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
        tls_mod.image = (void *)(base + ph_tls->p_vaddr);
        tls_mod.len = ph_tls->p_filesz;
        tls_mod.size = ph_tls->p_memsz;
        tls_mod.align = ph_tls->p_align;
        __add_tls(&tls_mod);
    }
    struct tls_data tls_data = __get_tls_data();
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
    __init_tp(__copy_tls(tlsbase, tls_data.size), hwcap, sysinfo);
}
weak_alias(__init_from_phdrs, static_init_from_phdrs);
