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
static const uint32_t *hashtab;
static const Verdef *verdef;
static const uint16_t *versym;
static size_t base;

static uint32_t elf_hash(const char *name)
{
    const unsigned char *u = (void *)name;
    uint_fast32_t h = 0;
    while (*u) {
        h = (h << 4) + *u++;
        h ^= (h >> 24) & 0xf0;
    }
    return h & 0x0fffffff;
}

hidden void __init_vdso(const void *p)
{
    if (!p) return;
    const Ehdr *eh = p;
    const Phdr *ph0 = (void *)((const char *)p + eh->e_phoff);
    const Phdr *ph = ph0;
    const size_t *dyn = 0;
    size_t phnum = eh->e_phnum, phent = eh->e_phentsize;
    for (; phnum; phnum--, ph = (void *)((const char *)ph + phent)) {
        if (ph->p_type == PT_DYNAMIC) {
            dyn = (const char *)p + ph->p_offset;
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
            case DT_VERDEF: verdef = dp; break;
            case DT_VERSYM: versym = dp; break;
        }
    }
    if (!versym) verdef = 0;
}

static int match_version(uint16_t versym, const char *ver, uint32_t hash)
{
    versym &= 0x7fff;
    Verdef *def = verdef;
    for (;;) {
        if (!(def->vd_flags & VER_FLG_BASE)
                && (def->vd_ndx & 0x7fff) == versym)
            break;

        if (def->vd_next == 0) return 0;
        def = (void *)((char *)def + def->vd_next);
    }
    Verdaux *va = (void *)((char *)def + def->vd_aux);
    return def->vd_hash == hash && !strcmp(ver, strings + va->vda_name);
}
#define OK_BINDS (1 << STB_GLOBAL | 1 << STB_WEAK)
#define OK_TYPES (1 << STT_FUNC)
hidden void *__vdsosym(const char *name, const char *ver)
{
    if (!strtab || !symtab || !hashtab) return 0;
    uint32_t h = elf_hash(name);
    uint32_t vh = 0;
    const uint32_t *bucket = hashtab + 2;
    const uint32_t *chain = bucket + hashtab[0];
    if (verdef) vh = elf_hash(ver);
    for (uint32_t idx = bucket[h % hashtab[0]]; idx; idx = chain[idx])
    {
        const Sym *sym = symtab + idx;
        if (((1 << ST_BIND(sym->st_info)) & OK_BINDS)
                && (1 << ST_TYPE(sym->st_info) & OK_TYPES)
                && sym->st_shndx != SHN_UNDEF
                && !strcmp(name, strtab + sym->st_name)
                && (!verdef || match_version(versym[idx], ver, vh)))
            return (void *)(base + sym->st_value);
    }
    return 0;
}
