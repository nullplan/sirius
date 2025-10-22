#include "vdso.h"
#include <elf.h>
#include <string.h>

#ifndef __SIXTY_FOUR
typedef Elf32_Ehdr Ehdr;
typedef Elf32_Phdr Phdr;
typedef Elf32_Sym Sym;
#define ST_TYPE ELF32_ST_TYPE
#define ST_BIND ELF32_ST_BIND
#else
typedef Elf64_Ehdr Ehdr;
typedef Elf64_Phdr Phdr;
typedef Elf64_Sym Sym;
#define ST_TYPE ELF64_ST_TYPE
#define ST_BIND ELF64_ST_BIND
#endif
typedef Elf32_Verdef Verdef; /* same type either way */
typedef Elf32_Verdaux Verdaux; /* same type either way */

static const char *strtab;
static const Sym *symtab;
static const Verdef *verdef;
static const uint16_t *versym;
static size_t base;
static size_t numsyms;

static size_t count_ghashtab(const uint32_t *gh)
{
    size_t nbuckets = gh[0];
    size_t symoffset = gh[1];
    size_t bloom_size = gh[2];
    const size_t *bloom = (void *)(gh + 4);
    const uint32_t *buckets = (void *)(bloom + bloom_size);
    const uint32_t *chain = buckets + nbuckets;
    size_t maxbucket = 0;
    for (size_t i = 0; i < nbuckets; i++)
        if (buckets[i] > maxbucket)
            maxbucket = buckets[i];
    size_t count = maxbucket;
    while (!(chain[count - symoffset] & 1))
        count++;
    return count + 1;
}

hidden void __init_vdso(const void *p)
{
    if (!p) return;
    const Ehdr *eh = p;
    const Phdr *ph0 = (void *)((const char *)p + eh->e_phoff);
    const Phdr *ph = ph0;
    const size_t *dyn = 0;
    const uint32_t *hashtab = 0;
    const uint32_t *ghashtab = 0;
    size_t phnum = eh->e_phnum, phent = eh->e_phentsize;
    for (; phnum; phnum--, ph = (void *)((const char *)ph + phent)) {
        if (ph->p_type == PT_DYNAMIC) {
            dyn = (void *)((const char *)p + ph->p_offset);
            base = (uintptr_t)dyn - ph->p_vaddr;
            break;
        }
    }
    if (!dyn) return;
    for (; *dyn; dyn += 2) {
        const void *dp = (const void *)(base + dyn[1]);
        switch (dyn[0]) {
            case DT_STRTAB: strtab = dp; break;
            case DT_SYMTAB: symtab = dp; break;
            case DT_HASH:   hashtab = dp; break;
            case DT_GNU_HASH: ghashtab = dp; break;
            case DT_VERDEF: verdef = dp; break;
            case DT_VERSYM: versym = dp; break;
        }
    }
    if (!versym) verdef = 0;
    if (hashtab) numsyms = hashtab[1];
    else if (ghashtab) numsyms = count_ghashtab(ghashtab);
}

static int match_version(uint16_t versym, const char *ver)
{
    versym &= 0x7fff;
    const Verdef *def = verdef;
    for (;;) {
        if (!(def->vd_flags & VER_FLG_BASE)
                && (def->vd_ndx & 0x7fff) == versym)
            break;

        if (def->vd_next == 0) return 0;
        def = (void *)((char *)def + def->vd_next);
    }
    Verdaux *va = (void *)((char *)def + def->vd_aux);
    return !strcmp(ver, strtab + va->vda_name);
}
#define OK_BINDS (1 << STB_GLOBAL | 1 << STB_WEAK)
#define OK_TYPES (1 << STT_FUNC)
hidden void *__vdsosym(const char *name, const char *ver)
{
    if (!strtab || !symtab) return 0;
    for (uint32_t idx = 1; idx < numsyms; idx++)
    {
        const Sym *sym = symtab + idx;
        if (((1 << ST_BIND(sym->st_info)) & OK_BINDS)
                && (1 << ST_TYPE(sym->st_info) & OK_TYPES)
                && sym->st_shndx != SHN_UNDEF
                && !strcmp(name, strtab + sym->st_name)
                && (!verdef || match_version(versym[idx], ver)))
            return (void *)(base + sym->st_value);
    }
    return 0;
}
