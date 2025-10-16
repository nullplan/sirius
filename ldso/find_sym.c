#include "ldso.h"
#include <string.h>
static uint32_t gnu_hash(const char *name)
{
    const unsigned char *u = (void *)name;
    uint_fast32_t h = 5381;
    while (*u)
        h = 33 * h + *u++;
    return h;
}

static uint32_t sysv_hash(const char *name)
{
    const unsigned char *u = (void *)name;
    uint_fast32_t h = 0;
    while (*u) {
        h = (h << 4) + *u++;
        h ^= (h >> 24) & 0xf0;
    }
    return h & 0x0fffffff;
}

static size_t lookup_gnu(const struct dso *dso, const char *name, uint32_t gh)
{
    /* have I mentioned the GNU hash table is a bit more difficult? */
#define NBPW (8 * sizeof (size_t))
    size_t nbuckets = dso->ghashtab[0];
    size_t symoffset = dso->ghashtab[1];
    size_t bloom_size = dso->ghashtab[2];
    size_t bloom_shift = dso->ghashtab[3];
    const size_t *bloom = (void *)(dso->ghashtab + 4);
    const uint32_t *buckets = (void *)(bloom + bloom_size);
    const uint32_t *chain = buckets + nbuckets;

    size_t bloomword = bloom[(gh / NBPW) % bloom_size];
    size_t test = (1ul << (gh % NBPW)) | (1ul << ((gh >> bloom_shift) % NBPW));
    if ((bloomword & test) != test) return 0;

    size_t idx = buckets[gh % nbuckets];
    if (!idx) return 0;
    idx--;
    gh |= 1;

    do {
        idx++;
        if ((chain[idx - symoffset] | 1) == gh) {
            const Sym *sym = dso->symtab + idx;
            if (!strcmp(dso->strtab + sym->st_name, name))
                return idx;
        }
    } while (!(chain[idx - symoffset] & 1));
    return 0;
}

static size_t lookup_sysv(const struct dso *dso, const char *name, uint32_t h)
{
    const uint32_t *bucket = dso->hashtab + 2;
    const uint32_t *chain = bucket + dso->hashtab[0];
    for (size_t idx = bucket[h % dso->hashtab[0]]; idx; idx = chain[idx])
    {
        const Sym *sym = dso->symtab + idx;
        if (!strcmp(dso->strtab + sym->st_name, name))
            return idx;
    }
    return 0;
}

#define OK_BINDS        (1ul << STB_GLOBAL | 1ul << STB_WEAK)
#define OK_TYPES        (1ul << STT_NOTYPE  | 1ul << STT_OBJECT | 1ul << STT_FUNC | 1ul << STT_COMMON | 1ul << STT_TLS)
hidden struct symdef __dl_find_sym(const char *name, struct dso *ctx, int need_defined)
{
    uint32_t gh = gnu_hash(name);
    int did_sysv = 0;
    uint32_t h;
    size_t i;
    for (; ctx; ctx = ctx->next)
    {
        i = 0;
        if (ctx->ghashtab)
            i = lookup_gnu(ctx, name, gh);
        else if (ctx->hashtab) {
            if (!did_sysv) {
                h = sysv_hash(name);
                did_sysv = 1;
            }
            i = lookup_sysv(ctx, name, h);
        }
        if (i)
        {
            const Sym *sym = ctx->symtab + i;
            if (((1ul << (sym->st_info >> 4)) & OK_BINDS)
                    && ((1ul << (sym->st_info & 0xf)) & OK_TYPES)
                    && (sym->st_shndx || (!need_defined && (sym->st_info & 0xf) != STT_TLS))
                    && (sym->st_value || (sym->st_info & 0xf) == STT_TLS))

                return (struct symdef){ctx, sym};
        }
    }
    return (struct symdef){0};
}
