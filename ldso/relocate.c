#include "ldso.h"
#include <sys/mman.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

#ifndef TLSDESC_BACKWARDS
#define TLSDESC_BACKWARDS 0
#endif

extern hidden const char __tlsdesc_dynamic[], __tlsdesc_static[];
static int process_relocs(struct dso *dso, const size_t *rel, size_t relsz, size_t stride, int (*err)(const char *, ...))
{
    int skip_rels = dso == __libc_object();
    int reuse_addends = skip_rels && stride == 2;
    size_t addend_idx = 0;
    struct dso *head = __dl_head();

    for (; relsz; relsz -= stride * sizeof (size_t), rel += stride)
    {
        int type = R_TYPE(rel[1]);
        if (!type) continue; /* type == 0 is the NONE relocation on all archs I have seen so far. */
        if (skip_rels && type == REL_RELATIVE)
            continue;

        size_t *rel_addr = (void *)(dso->base + rel[0]);
        size_t addend;
        if (stride == 3)
            addend = rel[2];
        else if (type == REL_GOT || type == REL_PLT || type == REL_COPY)
            addend = 0;
        else if (reuse_addends)
            addend = __libc_addend(addend_idx++);
        else
            addend = *rel_addr;

        size_t symval = 0;
        size_t tlsval = 0;
        const Sym *usym = 0;
        struct symdef def = {0};
        if (R_SYM(rel[1])) {
            usym = dso->symtab + R_SYM(rel[1]);
            def = usym->st_info >> 4 == STB_LOCAL?
                (struct symdef){dso, usym} :
                __dl_find_sym(dso->strtab + usym->st_name, type == REL_COPY? head->next : head, type == REL_PLT);
            if (def.sym) {
                symval = (size_t)(def.dso->base + def.sym->st_value);
                tlsval = def.sym->st_value;
            }
            if (!def.sym && (usym->st_info >> 4) != STB_WEAK) {
                if (err("error relocating `%s': symbol `%s' not found", dso->shortname, dso->strtab + usym->st_name))
                    return -1;
                continue;
            }
        }
        switch (type) {
            case REL_RELATIVE:
                *rel_addr = (size_t)dso->base + addend;
                break;

            case REL_SYMBOLIC:
            case REL_PLT:
            case REL_GOT:
                *rel_addr = symval + addend;
                break;

            case REL_USYMBOLIC:
                /* like SYMBOLIC, but possibly misaligned, so: */
                memcpy(rel_addr, &(size_t){symval + addend}, sizeof (size_t));
                break;

            case REL_COPY:
                if (usym->st_size != def.sym->st_size)
                {
                    if (err("error relocating `%s': Copy rel size mismatch (expected %zu, got %zu)", dso->shortname, usym->st_size, def.sym->st_size))
                        return -1;
                }
                else
                    memcpy(rel_addr, (void *)symval, usym->st_size);
                break;

            case REL_PCREL32:
                *(uint32_t *)rel_addr = symval + addend - (size_t)rel_addr;
                break;

            case REL_DTPMOD:
                *rel_addr = def.dso->tlsid;
                break;

            case REL_DTPOFF:
                *rel_addr = tlsval + addend - DTV_OFFSET;
                break;

            case REL_TPOFF:
                if (def.dso->tlsid > __dl_static_tls_cnt())
                {
                    if (err("error relocating `%s': static-TLS reloc against `%s' in dlopen module", dso->shortname, dso->strtab + usym->st_name))
                        return -1;
                }
                else {
#ifdef TLS_VARIANT_2
                    *rel_addr = tlsval - def.dso->tlsoff + addend;
#else
                    *rel_addr = tlsval + def.dso->tlsoff - sizeof (struct __pthread) - TP_OFFSET + addend;
#endif
                }
                break;

            case REL_TLSDESC:
                if (stride < 3)
                    addend = rel_addr[!TLSDESC_BACKWARDS];

                if (def.dso->tlsid > __dl_static_tls_cnt()) {
                    size_t *desc = __libc_malloc(2 * sizeof (size_t));
                    if (!desc)
                    {
                        if (err("error relocating `%s': Out of memory for TLS descriptor", dso->shortname))
                            return -1;
                    }
                    else {
                        rel_addr[TLSDESC_BACKWARDS] = (size_t)__tlsdesc_dynamic;
                        rel_addr[!TLSDESC_BACKWARDS] = (size_t)desc;
                        desc[0] = def.dso->tlsid;
                        desc[1] = tlsval + addend - DTV_OFFSET;
                    }
                } else {
                    rel_addr[TLSDESC_BACKWARDS] = (size_t)__tlsdesc_static;
#ifdef TLS_VARIANT_2
                    rel_addr[!TLSDESC_BACKWARDS] = tlsval - def.dso->tlsoff + addend;
#else
                    rel_addr[!TLSDESC_BACKWARDS] = tlsval + def.dso->tlsoff - sizeof (struct __pthread) - TP_OFFSET + addend;
#endif
                }
                break;


            default:
                if (err("error relocating `%s': Unknown relocation type %d", dso->shortname, type))
                    return -1;
                break;
        }
    }
    return 0;
}

static int process_relr(size_t base, const size_t *rel, size_t sz, const char *name, int (*err)(const char *, ...))
{
    size_t *rel_addr;
    if (sz && (*rel & 1)) {
        return err("Error relocating `%s': RELR table starts with bit field", name);
    }
    for (; sz; sz -= sizeof (size_t), rel++)
    {
        size_t w = *rel;
        if (!(w & 1)) {
            rel_addr = (void *)(base + w);
            *rel_addr++ += base;
        } else {
            w >>= 1;
            for (int cnt = 8 * sizeof (size_t) - 1; cnt; cnt--, w >>= 1) {
                if (w & 1)
                    *rel_addr += base;
                rel_addr++;
            }
        }
    }
    return 0;
}

hidden int __relocate(struct dso *dso, int (*err)(const char *, ...))
{
    int rv = 0;
    if (dso->relocated) return rv;
    size_t dyn[DT_RELR + 1] = {0};
    __decode_vec(dyn, dso->dynv, DT_RELR + 1);
    rv = process_relocs(dso, (void *)(dso->base + dyn[DT_REL]), dyn[DT_RELSZ], 2, err);
    if (!rv) rv = process_relocs(dso, (void *)(dso->base + dyn[DT_RELA]), dyn[DT_RELASZ], 3, err);
    if (!rv) rv = process_relocs(dso, (void *)(dso->base + dyn[DT_JMPREL]), dyn[DT_PLTRELSZ], dyn[DT_PLTREL] == DT_REL? 2 : 3, err);
    /* relr table is all relative, so we skip it for libc, since it has already been processed. */
    if (!rv && dso != __libc_object())
        rv = process_relr((size_t)dso->base, (void *)(dso->base + dyn[DT_RELR]), dyn[DT_RELRSZ], dso->name, err);
    if (!rv)
        dso->relocated = 1;
    if (!rv && dso->relro_start != dso->relro_end
            && mprotect(dso->base + dso->relro_start, dso->relro_end - dso->relro_start, PROT_READ))
    {
        err("Error relocation `%s': RELRO protection failed: %m", dso->name);
        rv = -1;
    }
    return rv;
}

hidden int __reloc_all(struct dso *dso, int (*err)(const char *, ...))
{
    for (; dso; dso = dso->next)
    {
        if (__relocate(dso, err))
            return -1;
    }
    return 0;
}

