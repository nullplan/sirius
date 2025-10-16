#include "ldso.h"
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include "libc.h"

#define PAGE_RND_DOWN(x) ((x) & -PAGE_SIZE)
#define PAGE_RND_UP(x) PAGE_RND_DOWN((x) + PAGE_SIZE - 1)

static int prot_from_flags(int flg)
{
    int rv = 0;
    if (flg & PF_R) rv |= PROT_READ;
    if (flg & PF_W) rv |= PROT_WRITE;
    if (flg & PF_X) rv |= PROT_EXEC;
    return rv;
}

hidden void *__map_library(int fd, struct dso *dso)
{
    union {
        Ehdr eh;
        char b[1024];
    } buf;
    ssize_t rd = read(fd, buf.b, sizeof buf);
    if (rd < 0) {
        __dl_print_error("`%s': Read error: %m", dso->name);
        return 0;
    }

    if ((size_t)rd < sizeof (Ehdr)) {
        __dl_print_error("`%s': File too short", dso->name);
        return 0;
    }

    if (memcmp(buf.b, "\177ELF", 4)
            || buf.eh.e_ident[EI_CLASS] != ELFCLASS
            || buf.eh.e_machine != ELFMACH
            || (buf.eh.e_type != ET_DYN && buf.eh.e_type != ET_EXEC)) {
        __dl_print_error("`%s': Bad magic", dso->name);
        return 0;
    }

    Phdr *ph0, *ph;
    int elftype = buf.eh.e_type;
    dso->phent = buf.eh.e_phentsize;
    dso->phnum = buf.eh.e_phnum;
    if (buf.eh.e_phoff + dso->phnum * dso->phent <= rd)
        ph0 = (void *)(buf.b + buf.eh.e_phoff);
    else {
        rd = pread(fd, buf.b, dso->phent * dso->phnum, buf.eh.e_phoff);
        if (rd != dso->phent * dso->phnum) {
            __dl_print_error("`%s: Failed to read program headers", dso->name);
            return 0;
        }
        ph0 = (void *)buf.b;
    }

    size_t min_address = SIZE_MAX;
    size_t max_address = 0;
    size_t min_offset = 0;
    int minflags = 0;
    ph = ph0;
    for (size_t i = 0; i < dso->phnum; i++, ph = (void *)((char *)ph + dso->phent)) {
        if (ph->p_type == PT_LOAD) {
            if (ph->p_vaddr < min_address) {
                min_address = ph->p_vaddr;
                minflags = ph->p_flags;
                min_offset = ph->p_offset;
            }
            if (ph->p_vaddr + ph->p_memsz > max_address)
                max_address = ph->p_vaddr + ph->p_memsz;
        }
    }

    min_address &= -PAGE_SIZE;
    min_offset &= -PAGE_SIZE;
    max_address = (max_address + PAGE_SIZE - 1) & -PAGE_SIZE;
    /* we REALLY require the ELF header to be mapped, so if the lowest LOAD segment has a nonzero offset,
     * reduce min_address by it and always start mapping at offset 0 */

    void *map = mmap((void *)(min_address - min_offset), max_address - min_address + min_offset, prot_from_flags(minflags), MAP_PRIVATE, fd, 0);
    if (map == MAP_FAILED) {
        __dl_print_error("Error loading `%s': %m", dso->name);
        return 0;
    }

    if (elftype == ET_EXEC && map != (void *)(min_address - min_offset)) {
        __dl_print_error("Error loading `%s': Non-relocatable object could not be loaded to preferred address", dso->name);
        goto out_unmap;
    }
    dso->base = (char *)map - min_address + min_offset;
    dso->phdr = (void *)((char *)map + ((Ehdr*)map)->e_phoff);

    ph = ph0;
    for (size_t i = 0; i < dso->phnum; i++, ph = (void *)((char *)ph + dso->phent)) {
        if (ph->p_type == PT_DYNAMIC) {
            dso->dynv = (void *)(dso->base + ph->p_vaddr);
        }
        if (ph->p_type == PT_LOAD) {
            size_t this_min = PAGE_RND_DOWN(ph->p_vaddr);
            size_t this_off = PAGE_RND_DOWN(ph->p_offset);
            size_t this_max = PAGE_RND_UP(ph->p_vaddr);
            /* reuse first segment */
            if (this_min != min_address
                    && mmap(dso->base + this_min, this_max - this_min, prot_from_flags(ph->p_flags), MAP_PRIVATE | MAP_FIXED, fd, this_off) == MAP_FAILED) {
                __dl_print_error("Error loading `%s': %m", dso->name);
                goto out_unmap;
            }
            if (ph->p_memsz > ph->p_filesz && (ph->p_flags & PF_W)) {
                char *eoi = dso->base + ph->p_vaddr + ph->p_filesz;
                char *pagebrk = (char *)PAGE_RND_UP((uintptr_t)eoi);
                memset(eoi, 0, pagebrk - eoi);
                if (ph->p_memsz - ph->p_filesz > pagebrk - eoi
                        && mmap(pagebrk, PAGE_RND_UP(ph->p_memsz - ph->p_filesz - (pagebrk - eoi)), prot_from_flags(ph->p_flags), MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0) == MAP_FAILED) {
                    __dl_print_error("Error loading `%s': %m", dso->name);
                    goto out_unmap;
                }
            }
        }
        if (ph->p_type == PT_GNU_RELRO) {
            dso->relro_start = PAGE_RND_DOWN(ph->p_vaddr);
            dso->relro_end = PAGE_RND_UP(ph->p_vaddr + ph->p_memsz);
        }
    }
    __process_dynv(dso);
    if (!dso->symtab || !dso->strtab || (!dso->hashtab && !dso->ghashtab))
    {
        __dl_print_error("Error loading `%s': Library lacks vital data.", dso->name);
        goto out_unmap;
    }
    dso->map = map;
    dso->map_len = max_address - min_address + min_offset;
    return map;

out_unmap:
    munmap(map, max_address - min_address + min_offset);
    return 0;
}
